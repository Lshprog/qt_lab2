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

    mymodel->addInfoFromDialog(modelindex,mymodel->getHyperlinkFromIndex(modelindex));

    return true;

}

void MyFilterModel::makelisthypelinks(QList<QString> *list)
{
    mymodel->makelisthypelinks(list);
}

bool MyFilterModel::addCategory(const QModelIndex &index)
{
    QModelIndex modelindex= mapToSource(index);

    mymodel->addInfoFromDialogCat(modelindex,mymodel->getHyperlinkFromIndex(modelindex));

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

void MyFilterModel::readFile(QString filename,int mode)
{
    if(mode==0){
        beginResetModel();
        mymodel->readFile(filename);
        endResetModel();
    }
    else if(mode == 1){
        beginResetModel();
        mymodel->cleanup();
        mymodel->readFile(filename);
        endResetModel();
    }

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

}
