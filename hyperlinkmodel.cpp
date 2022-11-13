#include "hyperlinkmodel.h"
#include <QFont>
#include <QColor>


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

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
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

bool HyperlinkModel::makeListInfo(QList<QString> *list)
{
    for (int i=0;i<rootHyperlink->getChildrenSize();i++){
        rootHyperlink->child(i)->listInfo(list,0);
    }

}





