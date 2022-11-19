#include "hyperlinkmodel.h"
#include <QFont>
#include <QColor>
<<<<<<< Updated upstream

=======
#include <QMessageBox>
#include <QMimeData>
#include <QCoreApplication>
>>>>>>> Stashed changes

HyperlinkModel::HyperlinkModel(QObject *parent)
    : QAbstractItemModel{parent}
{
     QVector<QVariant> rootData;
     rootData<<"Name"<<"Hyperlink"<<"Description";
     rootHyperlink = new Hyperlink(rootData);
     readFile();
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
        return hyperlink->data(index.column());
    }
    else if(role == HyperlinkRoles::CategoryRole){
        Hyperlink *hyperlink = static_cast<Hyperlink*>(index.internalPointer());
        return hyperlink->getCategoryStatus();
    }
    else if (role == Qt::ForegroundRole && index.column() == 1)
            return QColor(Qt::blue);
    else if (role == Qt::FontRole && index.column() == 1){
        QFont font;
        font.setUnderline(true);
        return font;
    }
    else if(role == Qt::FontRole && index.column() == 0){
        Hyperlink *hyperlink = static_cast<Hyperlink*>(index.internalPointer());
        if(hyperlink->getCategoryStatus()){
            QFont font;
            font.setBold(true);
            return font;
        }
        return QVariant();
    }
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
        return Qt::ItemIsEnabled;

<<<<<<< Updated upstream
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
=======
    Hyperlink *cur = getHyperlinkFromIndex(index);
    if(cur->getCategoryStatus()&&index.column()==1)
        return QAbstractItemModel::flags(index)|Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;;



    return Qt::ItemIsEditable | QAbstractItemModel::flags(index)|Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
>>>>>>> Stashed changes
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

void HyperlinkModel::readFile()
{
    //QString filename = "C:/Users/onisa/source/repos/qt_project_2/qt-lab2-lastversion/data/familytree1.txt";;
    //QString filename = "/Users/oleksiionishchenko/Documents/qtprojects/qt_lab2/data/familytree1.txt";
    QString filename = "D:/useless/tocompare.txt";
    QFile inputFile(filename);

    if(inputFile.open(QIODevice::ReadOnly)){
        int lastIndentation = 0;

        Hyperlink *lastParent = rootHyperlink;
        Hyperlink *lastHyperlink = 0;

        QTextStream in(&inputFile);

        while(!in.atEnd()){
            QString line = in.readLine();

            int currentIndentation = line.count("\t");

            //qDebug()<<currentIndentation;

            QVector<QVariant> infoList = getInfo(line);

            int diffIndent = currentIndentation - lastIndentation;

            if(diffIndent == 0){
                Hyperlink *hyperlink = new Hyperlink(infoList,lastParent);
                lastParent->appendChild(hyperlink);
                lastHyperlink = hyperlink;
                //qDebug()<<hyperlink->data(0)<<"1";

            }
            else if(diffIndent > 0){
                if(!lastHyperlink->getCategoryStatus())
                    lastHyperlink->setCategoryStatus(true);

                lastParent = lastHyperlink;
                Hyperlink *hyperlink = new Hyperlink(infoList,lastParent);
                //qDebug()<<hyperlink->data(0)<<"2";
                lastParent->appendChild(hyperlink);
                lastHyperlink = hyperlink;
            }
            else{
                int iterations = -diffIndent;
                for(int i=0;i<iterations;i++)
                    lastParent = lastParent->parentHyperlink();

                Hyperlink *hyperlink = new Hyperlink(infoList,lastParent);
                //qDebug()<<hyperlink->data(0)<<"3";
                lastParent->appendChild(hyperlink);
                lastHyperlink = hyperlink;

            }

            lastIndentation = currentIndentation;

        }

        inputFile.close();
    }
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

void HyperlinkModel::addInfoFromDialog(const QModelIndex &index, Hyperlink *parent, bool status)
{
    QVector<QVariant> data;

    Dialog win(nullptr);

    win.show();

    if(!win.exec())
        return;

    data<<win.returnName()<<win.returnLink()<<win.returnDescription();

    Hyperlink* new_hyperlink = new Hyperlink(data,parent);
    new_hyperlink->setCategoryStatus(status);

    //mymodel->insertnewrowchild(endrow,index.parent(),new_hyperlink);
    if(status)
        insertnewrowchild(0,index,new_hyperlink);
    else
        insertnewrowchild(parent->getChildrenSize(),index,new_hyperlink);
}



bool HyperlinkModel::insertnewrowchild(int row, const QModelIndex &parent, Hyperlink *link)
{
    Hyperlink *hyperlinkParent = getHyperlinkFromIndex(parent);
    bool success;

//    while((hyperlinkParent->getCategoryStatusOfChild(row)) && (row<hyperlinkParent->getChildrenSize())){
//        row++;
//    }

    beginInsertRows(parent,row,row);
    success = hyperlinkParent->insertChild(row,link);
    endInsertRows();

    return success;
}

Hyperlink *HyperlinkModel::returnroot() const
{
    return rootHyperlink;
}


QVector<QVariant> HyperlinkModel::getInfo(QString lineString)
{
    QString cleanedUpStr = lineString.trimmed();
    QStringList split = cleanedUpStr.split("::");

    QVector<QVariant> data;
    for(int i =0; i<split.size();i++)
        data << split[i];

    return data;
}


static const char s_treeNodeMimeType[] = "application/my.custom.model";
//static const char s_treeNodeMimeType[] = "application/x-hyperlink";


QStringList HyperlinkModel::mimeTypes() const
{
    //return QAbstractItemModel::mimeTypes()<<"application/vnd.text.list";
    //return QAbstractItemModel::mimeTypes()<<s_treeNodeMimeType;
    return QStringList()<<s_treeNodeMimeType;

}


QMimeData *HyperlinkModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData;
    QByteArray data;

    QDataStream stream(&data,QIODevice::WriteOnly);

    QList<Hyperlink*> nodes;

    for(const QModelIndex &index:indexes){
        qDebug()<<"mimeData first loop";
        Hyperlink *cur_node = getHyperlinkFromIndex(index);
        qDebug()<<cur_node->data(0)<<cur_node->data(2);
        if(!nodes.contains(cur_node)){
            nodes<<cur_node;
            qDebug()<<"mimeData first loop inner check";
        }
    }

    stream<<QCoreApplication::applicationPid();
    stream<<nodes.count();
    qDebug()<<nodes.count();
    for(Hyperlink *node:nodes){
        qDebug()<<"mimeData second loop";
        stream<<reinterpret_cast<qlonglong>(node);
    }
    mimeData->setData(s_treeNodeMimeType,data);
    return mimeData;


}

bool HyperlinkModel::dropMimeData(const QMimeData *mimeData, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_ASSERT(action == Qt::MoveAction);
    Q_UNUSED(column);
    qDebug()<<"here";
    //test if the data type is the good one
    if (!mimeData->hasFormat(s_treeNodeMimeType)) {
        qDebug()<<"nowhere_1";
        return false;
    }
    QByteArray data = mimeData->data(s_treeNodeMimeType);
    QDataStream stream(&data, QIODevice::ReadOnly);
    qint64 senderPid;
    stream >> senderPid;
    qDebug()<<senderPid;
    if (senderPid != QCoreApplication::applicationPid()) {
        // Let's not cast pointers that come from another process...
        qDebug()<<"nowhere_2";
        return false;
    }
    Hyperlink *parentNode = getHyperlinkFromIndex(parent);
    Q_ASSERT(parentNode);
    int count;
    stream >> count;
    qDebug()<<"shits "<<count;
    if (row == -1) {
        // valid index means: drop onto item. I chose that this should insert
        // a child item, because this is the only way to create the first child of an item...
        // This explains why Qt calls it parent: unless you just support replacing, this
        // is really the future parent of the dropped items.
        if (parent.isValid())
            row = 0;
        else
            // invalid index means: append at bottom, after last toplevel
            row = rowCount(parent);
    }
    for (int i = 0; i < count; ++i) {
        // Decode data from the QMimeData
        qlonglong nodePtr;
        stream >> nodePtr;
        Hyperlink *node = reinterpret_cast<Hyperlink *>(nodePtr);

        // Adjust destination row for the case of moving an item
        // within the same parent, to a position further down.
        // Its own removal will reduce the final row number by one.
        if (node->row() < row && parentNode == node->parentHyperlink());
            --row;

        // Remove from old position
        //delete node;
        //removeNode(node);

        Hyperlink *parent_cur = node->parentHyperlink();
        QList<QVariant> data;
        data<<node->data(0)<<node->data(1)<<node->data(2);
        Hyperlink *newHyperlink = new Hyperlink(data,parentNode);
        newHyperlink->setCategoryStatus(node->getCategoryStatus());
        for(int i = 0;i<node->getChildrenSize();i++){
            newHyperlink->appendChild(node->child(i));
        }
        //parent_cur->removeChild(node->row());
        //qDebug() << "Inserting into" << parent << row;

        delete node;

        // Insert at new position
        qDebug() << "Inserting into" << parent << row;
        //beginInsertRows(parent, row, row);
        beginInsertRows(parent,row,row);
        parentNode->insertChild(row, newHyperlink);
        endInsertRows();
        //endInsertRows();
        ++row;
    }
    return true;
}

bool HyperlinkModel::makeListInfo(QList<QString> *list)
{
    for (int i=0;i<rootHyperlink->getChildrenSize();i++){
        rootHyperlink->child(i)->listInfo(list,0);
    }

}

Qt::DropActions HyperlinkModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions HyperlinkModel::supportedDragActions() const
{
    return Qt::MoveAction;
}
