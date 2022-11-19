#include "myfiltermodel.h"

MyFilterModel::MyFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
    mymodel = new HyperlinkModel(this);

    this->setSourceModel(mymodel);

    datalist<<""<<""<<"";
}

bool MyFilterModel::filterAcceptsRow(int source_row, const QModelIndex & source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    return hasToBeDisplayed(index);
}

bool MyFilterModel::addHyperlink(const QModelIndex &index)
{
    QModelIndex modelindex= mapToSource(index);

    //Hyperlink* parent = mymodel->getHyperlinkFromIndex(modelindex);

    mymodel->addInfoFromDialog(modelindex,mymodel->getHyperlinkFromIndex(modelindex),false);

    return true;

}

void MyFilterModel::makelisthypelinks(QList<QString> *list)
{
    mymodel->makelisthypelinks(list);
}

bool MyFilterModel::addCategory(const QModelIndex &index)
{
    QModelIndex modelindex= mapToSource(index);

    mymodel->addInfoFromDialog(modelindex,mymodel->getHyperlinkFromIndex(modelindex),true);

    return true;
}

bool MyFilterModel::editData(const QModelIndex &index)
{

    QModelIndex modelindex= mapToSource(index);

    return true;
}

bool MyFilterModel::checkCategoryStatus(const QModelIndex &index)
{
    QModelIndex modelindex = mapToSource(index);
    Hyperlink* current = mymodel->getHyperlinkFromIndex(modelindex);

    return current->getCategoryStatus();
}

bool MyFilterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QModelIndex modelindex= mapToSource(index);

    Hyperlink *cur = mymodel->getHyperlinkFromIndex(modelindex);

    bool result = cur->setData(index.column(),value);

    //bool result = mymodel->setData(modelindex,value,role);

    if(result)
        emit dataChanged(index,index,{role});

    return result;
}

bool MyFilterModel::makeFileInfo(QList<QString> *list)
{
    return mymodel->makeListInfo(list);
}

bool MyFilterModel::hasToBeDisplayed(const QModelIndex index) const
{

    bool result = false;
        // How many child this element have
    if ( sourceModel()->rowCount(index) > 0 )
    {
        for( int ii = 0; ii < sourceModel()->rowCount(index); ii++)
        {
            QModelIndex childIndex = sourceModel()->index(ii,0,index);
            if ( ! childIndex.isValid() )
                break;
            result = hasToBeDisplayed(childIndex);
            if (result)
            {
                // there is atless one element to display
                break;
            }
        }
    }
    else
    {

        QList<QModelIndex> list1;
        list1<<sourceModel()->index(index.row(), 0, index.parent())<<sourceModel()->index(index.row(), 1, index.parent())
            <<sourceModel()->index(index.row(), 2, index.parent());
        QList<QString> type1;
        type1<<sourceModel()->data(list1[0],Qt::DisplayRole).toString()<<sourceModel()->data(list1[1], Qt::DisplayRole).toString()
                <<sourceModel()->data(list1[2], Qt::DisplayRole).toString();

        //QRegularExpression re;
        QRegularExpression re;

        for(int i = 0;i<3;i++){
            re.setPattern(datalist[i]);
            //qDebug()<<re;
            if(!type1[i].contains(re)){

                return false;
            }

        }
        return true;

    }
    return result;
<<<<<<< Updated upstream
=======
}

QStringList MyFilterModel::mimeTypes() const
{
    qDebug()<<"mimetypes in proxy";

    return mymodel->mimeTypes();
}

QMimeData *MyFilterModel::mimeData(const QModelIndexList &indexes) const
{
    qDebug()<<"mimedata in proxy";

        QModelIndexList list;
        for(const QModelIndex &index:indexes)
            list<<mapToSource(index);
        return mymodel->mimeData(list);
}

bool MyFilterModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    qDebug()<<"dropmime data in proxy";

    //beginResetModel();
    return mymodel->dropMimeData(data,action,row,column,mapToSource(parent));
    //endResetModel();
}

Qt::DropActions MyFilterModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions MyFilterModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

Qt::ItemFlags MyFilterModel::flags(const QModelIndex &index) const
{
    qDebug()<<"flags in proxy";
    return mymodel->flags(mapToSource(index));
}


bool MyFilterModel::checkIndexValue(const QModelIndex index) const
{
    bool result = true;

    QList<QModelIndex> list1;
    list1<<sourceModel()->index(index.row(), 0, index.parent())<<sourceModel()->index(index.row(), 1, index.parent())
        <<sourceModel()->index(index.row(), 2, index.parent());
    QList<QString> type1;
    type1<<sourceModel()->data(list1[0],Qt::DisplayRole).toString()<<sourceModel()->data(list1[1], Qt::DisplayRole).toString()
            <<sourceModel()->data(list1[2], Qt::DisplayRole).toString();

    QRegularExpression re;
    for(int i=0;i<3;i++){
        re.setPattern(datalist[i]);
        if(!type1[i].contains(re)){
            result = false;
            break;
        }
    }

    return result;

}

bool MyFilterModel::saveInfoToFile(QString filename)
{
   qDebug()<<"CHTONAXYI"<<"\n"<<"\n";
   QFile filecheck(filename);
   if(filecheck.open(QIODevice::WriteOnly | QIODevice::Text))
       filecheck.remove();

   QFile file(filename);
   file.open(QIODevice::WriteOnly | QIODevice::Text);

   QTextStream out(&file);

   QList<QString> hyperlinks;
   this->makeFileInfo(&hyperlinks);

   for(int i = 0;i<hyperlinks.size();i++){
       out<<hyperlinks[i].toUtf8()<<"\n";
   }

>>>>>>> Stashed changes

}
