#include "hyperlinkmodel.h"

HyperlinkModel::HyperlinkModel(QObject *parent)
    : QAbstractItemModel{parent}
{
     rootHyperlink = new Hyperlink("Name","Hyperlink","Description",false);
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
    return 3;
}

QVariant HyperlinkModel::data(const QModelIndex &index, int role) const
{

    if(!index.isValid())
        return QVariant();

    if (role!=Qt::DisplayRole)
        return QVariant();

    Hyperlink *hyperlink = static_cast<Hyperlink*>(index.internalPointer());

    return hyperlink->data(index.column());

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
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

bool HyperlinkModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role!=Qt::EditRole)
        return false;
    Hyperlink *hyperlink = static_cast<Hyperlink*>(index.internalPointer());

    bool result = hyperlink->setData(index.column(),value);

    if(result)
        emit dataChanged(index,index,{role});

    return result;
}

bool HyperlinkModel::insertRows(int row, int count, const QModelIndex &parent)
{

}

bool HyperlinkModel::removeRows(int row, int count, const QModelIndex &parent)
{

}



void HyperlinkModel::readFile()
{

    QString filename = "/Users/oleksiionishchenko/Documents/qtprojects/qt_lab2/data/familytree1.txt";
    QFile inputFile(filename);

    if(inputFile.open(QIODevice::ReadOnly)){
        int lastIndentation = 0;

        Hyperlink *lastParent = rootHyperlink;
        Hyperlink *lastHyperlink = nullptr;

        QTextStream in(&inputFile);

        while(!in.atEnd()){
            QString line = in.readLine();

            int currentIndentation = line.count("\t");

            //qDebug()<<currentIndentation;

            QStringList infoList = getInfo(line);

            int diffIndent = currentIndentation - lastIndentation;

            if(diffIndent == 0){
                Hyperlink *hyperlink = new Hyperlink(infoList[0],infoList[1],infoList[2],false,lastParent);
                lastParent->appendChild(hyperlink);
                lastHyperlink = hyperlink;

            }
            else if(diffIndent > 0){
                if(!lastHyperlink->getCategoryStatus())
                    lastHyperlink->setCategoryStatus(true);

                lastParent = lastHyperlink;
                Hyperlink *hyperlink = new Hyperlink(infoList[0],infoList[1],infoList[2],false,lastParent);
                lastParent->appendChild(hyperlink);

                lastHyperlink = hyperlink;
            }
            else{
                int iterations = -diffIndent;
                for(int i=0;i<iterations;i++)
                    lastParent = lastParent->parentHyperlink();

                Hyperlink *hyperlink = new Hyperlink(infoList[0],infoList[1],infoList[2],false,lastParent);
                lastParent->appendChild(hyperlink);
                lastHyperlink = hyperlink;

            }



            lastIndentation = currentIndentation;

        }
        inputFile.close();
    }
}

QStringList HyperlinkModel::getInfo(QString lineString)
{
    QString cleanedUpStr = lineString.trimmed();
    QStringList split = cleanedUpStr.split("::");

    return split;
}
