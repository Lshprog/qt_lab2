#include "hyperlinkmodel.h"
#include <QFont>
#include <QColor>
#include <QMessageBox>
#include <QMimeData>
#include <QCoreApplication>
#include <QMimeType>
#include <QAbstractItemModel>
#include <QQueue>

HyperlinkModel::HyperlinkModel(QObject *parent)
    : QAbstractItemModel{parent}
{
     QVector<QVariant> rootData;
     setFilterStatus(false);
     rootData<<"Name"<<"Hyperlink"<<"Description";
     rootHyperlink = new Hyperlink(rootData);
     rootHyperlink->setCategoryStatus(true);
     //readFile("filename");
}


HyperlinkModel::~HyperlinkModel()
{
    delete rootHyperlink;
}

QModelIndex HyperlinkModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row,column,parent))
        return QModelIndex();
    Hyperlink *parentHyperlink;
    if(!parent.isValid())
        parentHyperlink = rootHyperlink;
    else
        parentHyperlink = static_cast<Hyperlink*>(parent.internalPointer());

    Hyperlink *childHyperlink = parentHyperlink->child(row);

    if(childHyperlink)
        return createIndex(row,column,childHyperlink);

    return QModelIndex();


}

QModelIndex HyperlinkModel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
        return QModelIndex();

    Hyperlink *childHyperlink = static_cast<Hyperlink*>(child.internalPointer());
    Hyperlink *parentHyperlink = childHyperlink->parentHyperlink();

    if(parentHyperlink==rootHyperlink)
        return QModelIndex();

    return createIndex(parentHyperlink->row(),0,parentHyperlink);

}

int HyperlinkModel::rowCount(const QModelIndex &parent) const
{
    Hyperlink *parentHyperlink;

    if(parent.column()>0)
        return 0;
    if(!parent.isValid())
        parentHyperlink = rootHyperlink;
    else
        parentHyperlink = static_cast<Hyperlink*>(parent.internalPointer());

    return parentHyperlink->childCount();
}

int HyperlinkModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootHyperlink->columnCount();
}

QVariant HyperlinkModel::data(const QModelIndex &index, int role) const
{

    if(!index.isValid())
        return QVariant();

    if(role==Qt::DisplayRole || role== Qt::EditRole){
        Hyperlink *hyperlink = static_cast<Hyperlink*>(index.internalPointer());
        if(index.column()==1 && data(index,CategoryRole).toBool())
            return "";
        return hyperlink->data(index.column());
    }
    else if(role == HyperlinkRoles::CategoryRole){
        Hyperlink *hyperlink = static_cast<Hyperlink*>(index.internalPointer());
        return hyperlink->getCategoryStatus();
    }
    else if (role == Qt::ForegroundRole && index.column() == 1)
            return QColor(Qt::blue);
    else if (role == Qt::FontRole && index.column() == 1){
        if(!data(index,CategoryRole).toBool())
        {
            QFont font;
            font.setUnderline(true);
            //font.setItalic(true);
            font.setPointSize(11);
            font.setFamily("Times New Roman");
            return font;
        }
        return QVariant();
    }
    else if(role == Qt::FontRole && (index.column() == 0 || index.column() == 2)){
        Hyperlink *hyperlink = static_cast<Hyperlink*>(index.internalPointer());
        QFont font;
        if(hyperlink->getCategoryStatus()){
            font.setBold(true);
            font.setPointSize(12);
        }
        else{
            //font.setItalic(true);
            font.setPointSize(11);
        }
        font.setFamily("Times New Roman");
        //font.setPointSize(11);
        return font;
    }
//    else if(role == Qt::BackgroundRole){
//        if(data(index,CategoryRole).toBool())
//        {
//            return QBrush(Qt::lightGray);
//        }
//        return QVariant();
//    }
    else
        return QVariant();


}

QVariant HyperlinkModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && (role == Qt::DisplayRole)){
        return rootHyperlink->data(section);
    }

    return QVariant();
}

Qt::ItemFlags HyperlinkModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::ItemIsEnabled|Qt::ItemIsDropEnabled;
    Hyperlink *cur = getHyperlinkFromIndex(index);
    if(cur->getCategoryStatus()&&index.column()==1)
        return QAbstractItemModel::flags(index)|Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

    if(!cur->getCategoryStatus()){
         return Qt::ItemIsEditable | QAbstractItemModel::flags(index)|Qt::ItemIsDragEnabled;
    }


    return Qt::ItemIsEditable | QAbstractItemModel::flags(index)|Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;


}

bool HyperlinkModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if(role!=Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootHyperlink->setData(section, value);

    if(result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

bool HyperlinkModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role!=Qt::EditRole)
        return false;

    Hyperlink *hyperlink = getHyperlinkFromIndex(index);
    if(index.column()==1 && hyperlink->getCategoryStatus())
        return false;

    bool result = hyperlink->setData(index.column(),value);

    if(result)
        emit dataChanged(index,index,{role});

    return result;
}

bool HyperlinkModel::insertRows(int row,int count, const QModelIndex &parent)
{
    Hyperlink *hyperlinkParent = getHyperlinkFromIndex(parent);
    bool success;

    beginInsertRows(parent,row,row + count - 1);
    success = hyperlinkParent->insertChildren(row,count,rootHyperlink->columnCount());
    endInsertRows();

    return success;
}

bool HyperlinkModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Hyperlink *parentHyperlink = getHyperlinkFromIndex(parent);
    bool success = true;

    beginRemoveRows(parent,row,row+count-1);
    success = parentHyperlink->removeChildren(row,count);
    endRemoveRows();


    return success;
}

//bool HyperlinkModel::insertCategory(int row,const QModelIndex &parent)
//{
//    Hyperlink *hyperlinkParent = getHyperlinkFromIndex(parent);
//    bool success;

//    beginInsertRows(parent,row,row);
//    success = hyperlinkParent->insertChildren(row,1,rootHyperlink->columnCount());
//    endInsertRows();

//    return success;

//}

bool HyperlinkModel::readFile(QString filename)
{
    //QString filename = "C:/Users/onisa/source/repos/qt_project_2/qt-lab2-lastversion/data/familytree1.txt";;
    //QString filename = "/Users/oleksiionishchenko/Documents/qtprojects/qt_lab2/data/familytree1.txt";
    beginResetModel();
    qDebug()<<filename;
    QStringList split = filename.split(".");
    if(split[1]!="txt") return false;

    QFile inputFile(filename);

    if(inputFile.open(QIODevice::ReadOnly)){
        int lastIndentation = 0;

        Hyperlink *lastParent = rootHyperlink;
        Hyperlink *lastHyperlink = 0;

        QTextStream in(&inputFile);
        in.seek(0);

        while(!in.atEnd()){
            QString line = in.readLine();
            int currentIndentation = line.count("\t");

            //qDebug()<<currentIndentation;
            QPair<QVector<QVariant>,bool> pair = getInfo(line);

            if(pair.first.empty()){
                this->cleanup();
                return false;
            }

            QVector<QVariant> infoList = pair.first;
            bool temp_status = pair.second;

            int diffIndent = currentIndentation - lastIndentation;

            //qDebug()<<diffIndent;

            if(infoList.size()<1 || infoList.size()>4 || (diffIndent>1 && lastParent==rootHyperlink)||lastParent->getChildrenSize()>30){
                cleanup();
                QMessageBox msgBox;
                msgBox.setWindowTitle("ERROR!");
                msgBox.setText("Cant read this file!!!");
                //msgBox.setInformativeText();
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setIcon(QMessageBox::Critical);
                int ret = msgBox.exec();
                endResetModel();
                return false;
                }
            if(diffIndent == 0){
                Hyperlink *hyperlink = new Hyperlink(infoList,lastParent);
                hyperlink->setCategoryStatus(temp_status);
                if(lastParent==rootHyperlink)
                    hyperlink->setCategoryStatus(true);
                int cur_row = lastParent->checkDuplicates(hyperlink);
                if(cur_row!=lastParent->getChildrenSize()+1){
                    if(lastParent->getCategoryStatusOfChild(cur_row)){
                        lastParent = lastParent->child(cur_row);
                        lastHyperlink = lastParent;
                    }

                }
                else{

                    lastParent->appendChild(hyperlink);
                    lastHyperlink = hyperlink;
                }

            }
            else if(diffIndent > 0){
                if(!lastHyperlink->getCategoryStatus()){
                    lastHyperlink->setCategoryStatus(true);
                    if(lastHyperlink->columnCount()>1)
                        lastHyperlink->setData(1,"");
                }

                lastParent = lastHyperlink;
                Hyperlink *hyperlink = new Hyperlink(infoList,lastParent);
                hyperlink->setCategoryStatus(temp_status);
                int cur_row = lastParent->checkDuplicates(hyperlink);

                if(cur_row!=lastParent->getChildrenSize()+1){
                    if(lastParent->getCategoryStatusOfChild(cur_row)){
                        lastParent = lastParent->child(cur_row);
                        lastHyperlink = lastParent;
                    }
                }
                else{
                    lastParent->appendChild(hyperlink);
                    lastHyperlink = hyperlink;
                }
            }
            else{
                int iterations = -diffIndent;
                for(int i=0;i<iterations;i++)
                    lastParent = lastParent->parentHyperlink();

                Hyperlink *hyperlink = new Hyperlink(infoList,lastParent);
                hyperlink->setCategoryStatus(temp_status);
                int cur_row = lastParent->checkDuplicates(hyperlink);
                if(cur_row!=lastParent->getChildrenSize()+1){
                    if(lastParent->getCategoryStatusOfChild(cur_row)){
                        lastParent = lastParent->child(cur_row);
                        lastHyperlink = lastParent;
                    }

                }
                else{

                    lastParent->appendChild(hyperlink);
                    lastHyperlink = hyperlink;

                }

            }

            lastIndentation = currentIndentation;

        }
        endResetModel();
        inputFile.close();
        return true;
    }
    endResetModel();
    return false;
}

void HyperlinkModel::cleanup()
{
    delete rootHyperlink;
    QVector<QVariant> rootData;
    rootData<<"Name"<<"Hyperlink"<<"Description";
    rootHyperlink = new Hyperlink(rootData);
}

void HyperlinkModel::addTopLevelCategory()
{
    //rootHyperlink->appendChild();
    addInfoFromDialogCat(QModelIndex(),rootHyperlink);
}

bool HyperlinkModel::getFilterStatus()
{
    return filterStatus;
}

void HyperlinkModel::setFilterStatus(bool status)
{
    filterStatus = status;
}

QStringList HyperlinkModel::mimeTypes() const
{
    //return QStringList() << s_treeNodeMimeType;
    QStringList types;
    types << "text/plain";
    return types;
}

QMimeData *HyperlinkModel::mimeData(const QModelIndexList &indexes) const
{

    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
            QString text = data(index, Qt::DisplayRole).toString();
            stream << index.internalId() << index.row() << index.column() << text;

        }
    }

    mimeData->setData("text/plain", encodedData);
    return mimeData;
}


bool HyperlinkModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
   if (action == Qt::IgnoreAction)
       return true;

   if (!data->hasFormat("text/plain"))
       return false;

//   int beginRow;

//   if (row != -1)
//       beginRow = row;
//   else if (parent.isValid())
//       beginRow = 0;
//   else
//       beginRow = rowCount(QModelIndex());

   QByteArray encodedData = data->data("text/plain");
   QDataStream stream(&encodedData, QIODevice::ReadOnly);
   QHash<qint64, QMap<int,QHash<int,QString> > > newItems;
   QList<Hyperlink*> list;

   while (!stream.atEnd()) {
       qint64 id;
       int _row;
       int _column;
       QString text;
       stream >> id >> _row >> _column >> text;
       newItems[id][_row][_column] = text;
       QModelIndex index = createIndex(_row,_column,id);
       Hyperlink* cur = getHyperlinkFromIndex(index);
       if(!list.contains(cur))
           list<<cur;
   }
   //int rows = newItems.count();

   for(Hyperlink * cur_link:list){
       cur_link->showInfo();

       Hyperlink *oldparentlink = cur_link->parentHyperlink();
       Hyperlink *newparentlink = getHyperlinkFromIndex(parent);

       if(oldparentlink == newparentlink)
           return false;

       if(newparentlink==rootHyperlink && !cur_link->getCategoryStatus())
           return false;


       beginResetModel();

       int result = newparentlink->checkDuplicates(cur_link);
       if(result!=newparentlink->getChildrenSize()+1){
           Hyperlink* duplicatelink = newparentlink->child(result);
           QQueue<QPair<Hyperlink*,Hyperlink*>> queue;
           QPair<Hyperlink*,Hyperlink*> pairentry(cur_link,duplicatelink);
           queue.enqueue(pairentry);
           while(!queue.isEmpty())
           {
               QPair<Hyperlink*,Hyperlink*> pair = queue.dequeue();

               for(int i =0;i<pair.first->getChildrenSize();i++){
                   int check_row = pair.second->checkDuplicates(pair.first->child(i));
                   if(check_row!=pair.second->getChildrenSize()+1){
                       QPair<Hyperlink*,Hyperlink*> newpair(pair.first->child(i),pair.second->child(check_row));
                       queue.enqueue(newpair);
                   }
                   else{
                       int temp_row = pair.first->child(i)->row();
                       pair.first->child(i)->setParentHyperlink(pair.second);
                       pair.second->appendChild(pair.first->child(i));
                       pair.first->removechild(temp_row);

                   }
               }
               Hyperlink* parenttemp = pair.first->parentHyperlink();
               parenttemp->removechild(pair.first->row());
           }
       }
       else{
           int temp_row = cur_link->row();
           cur_link->setParentHyperlink(newparentlink);
           newparentlink->appendChild(cur_link);
           qDebug()<<cur_link->row();
           oldparentlink->removechild(temp_row);
       }

       endResetModel();
    }
   return true;
}

Qt::DropActions HyperlinkModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions HyperlinkModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

Hyperlink *HyperlinkModel::getHyperlinkFromIndex(const QModelIndex &index) const
{
    if(index.isValid()){
        Hyperlink *link = static_cast<Hyperlink*>(index.internalPointer());
        if(link)
            return link;
    }
    return rootHyperlink;
}


void HyperlinkModel::makelisthypelinks(QList<QString> *list)
{
    rootHyperlink->writeListOfLinks(list);

}

QModelIndex HyperlinkModel::addInfoFromDialog(const QModelIndex &index, Hyperlink *parent)
{
    QVector<QVariant> data;

    Dialog win(nullptr);

    win.show();

    if(!win.exec())
        return QModelIndex();

    data<<win.returnName()<<win.returnLink()<<win.returnDescription();

    Hyperlink* new_hyperlink = new Hyperlink(data,parent);   
    new_hyperlink->setCategoryStatus(false);

    int result = parent->checkDuplicates(new_hyperlink);

    if(result==parent->getChildrenSize()+1){
        insertnewrowchild(parent->getChildrenSize(),index,new_hyperlink);
        return QModelIndex();
    }

    QModelIndex cur = this->index(result,0,index);
    return cur;

}

QModelIndex HyperlinkModel::addInfoFromDialogCat(const QModelIndex &index, Hyperlink *parent)
{
    QVector<QVariant> data;

    DialogCat win(nullptr);

    win.show();

    if(!win.exec())
        return QModelIndex();

    data<<win.returnName()<<""<<win.returnDescription();

    Hyperlink* new_hyperlink = new Hyperlink(data,parent);
    new_hyperlink->setCategoryStatus(true);

    int result = parent->checkDuplicates(new_hyperlink);

    if(result==parent->getChildrenSize()+1){
        int k = 0;
        while(k<parent->getChildrenSize() && parent->getCategoryStatusOfChild(k))
            k++;
        insertnewrowchild(k,index,new_hyperlink);
        return QModelIndex();
    }
    QModelIndex cur = this->index(result,0,index);
    return cur;


}



bool HyperlinkModel::insertnewrowchild(int row, const QModelIndex &parent, Hyperlink *link)
{
    Hyperlink *hyperlinkParent = getHyperlinkFromIndex(parent);
    bool success;

    beginInsertRows(parent,row,row);
    success = hyperlinkParent->insertChild(row,link);
    endInsertRows();

    return success;
}

Hyperlink *HyperlinkModel::returnroot() const
{
    return rootHyperlink;
}

//bool HyperlinkModel::removeHyperlink(Hyperlink *node)
//{
//    const int row = node->row();
//    QModelIndex idx = createIndex(row, 0, node);
//    beginRemoveRows(idx.parent(), row, row);
//    node->parentHyperlink()->removechild(row);
//    endRemoveRows();
//}

//bool HyperlinkModel::copyNodes(const QModelIndex &link1, const QModelIndex &newparentlink)
//{


//    QQueue<QPair<QModelIndex,QModelIndex>> queue;
//    QPair<QModelIndex,QModelIndex> pair(link1,newparentlink);
//    queue.enqueue(pair);

//    while(!queue.isEmpty()){

//        QPair<QModelIndex,QModelIndex> to_compare=queue.dequeue();

//        Hyperlink* hyp1 = getHyperlinkFromIndex(to_compare.first);
//        Hyperlink* newparenthyp = getHyperlinkFromIndex(to_compare.second);

//        int result = newparenthyp->checkDuplicates(hyp1);

//        if(result!=newparenthyp->getChildrenSize()+1){
//            const QModelIndex &takeindex = this->index(result,0,to_compare.second);
//            for(int i = 0;i<hyp1->getChildrenSize();i++){
//                copyNodes(this->index(i,0,to_compare.first),takeindex);
//            }
//        }
//        else{

//            QList<QVariant> data;
//            data<<hyp1->data(0)<<hyp1->data(1)<<hyp1->data(2);
//            Hyperlink* newnode = new Hyperlink(data,newparenthyp);
//            newnode->setCategoryStatus(hyp1->getCategoryStatus());

//            int new_row = newparenthyp->getNewRow(newnode->getCategoryStatus());
//            insertnewrowchild(new_row,newparentlink,newnode);

//            const QModelIndex &index = this->index(new_row,0,to_compare.second);

//            for(int i = 0;i<hyp1->getChildrenSize();i++){
//                copyNodes(this->index(i,0,to_compare.first),index);
//            }
//            //QQueue<QPair<QModelIndex,QModelIndex>> queue_internal;
//            //QPair<QModelIndex,QModelIndex> pair_internal(,newparentlink);
//            //queue_internal.enqueue(pair_internal);

//        }
//    }
//    Hyperlink* hyp1 = getHyperlinkFromIndex(link1);
//    Hyperlink* hyp1parent = hyp1->parentHyperlink();
//    beginRemoveRows(link1.parent(),hyp1->row(),hyp1->row());
//    hyp1parent->removechild(hyp1->row());
//    endRemoveRows();
//    return true;

//}


QPair<QVector<QVariant>,bool> HyperlinkModel::getInfo(QString lineString)
{
    QPair<QVector<QVariant>,bool> pair;
    QString cleanedUpStr = lineString.trimmed();
    QStringList split = cleanedUpStr.split("::");
    if(split.size()<2) return pair;
    //qDebug()<<"Splitsize: "<<split.size();
    QVector<QVariant> data;
    int i = 0;
    bool status = false;
    for(i; i<split.size();i++){
        if(i==3){
            if(split[i]=="true")
                status = true;
            else
                status = false;
        }
       else{
            data << split[i];
        }
    }

    pair.first = data;
    pair.second = status;
    //qDebug()<<data;
    return pair;
}

bool HyperlinkModel::makeListInfo(QList<QString> *list)
{
    for (int i=0;i<rootHyperlink->getChildrenSize();i++){
        rootHyperlink->child(i)->listInfo(list,0);
    }
    return true;

}





