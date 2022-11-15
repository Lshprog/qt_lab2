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

    //bool status = mymodel->getFilterStatus();

    if(!mymodel->getFilterStatus())
        return hasToBeDisplayed(index);

    return hasToBeDisplayedCat(index);
}

QModelIndex MyFilterModel::addHyperlink(const QModelIndex &index)
{
    QModelIndex modelindex= mapToSource(index);

    //Hyperlink* parent = mymodel->getHyperlinkFromIndex(modelindex);

    QModelIndex result =  mapFromSource(mymodel->addInfoFromDialog(modelindex,mymodel->getHyperlinkFromIndex(modelindex)));

    return result;

}

void MyFilterModel::makelisthypelinks(QList<QString> *list)
{
    mymodel->makelisthypelinks(list);
}

QModelIndex MyFilterModel::addCategory(const QModelIndex &index)
{
    QModelIndex modelindex= mapToSource(index);

    QModelIndex result = mapFromSource(mymodel->addInfoFromDialogCat(modelindex,mymodel->getHyperlinkFromIndex(modelindex)));

    return result;
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

bool MyFilterModel::readFile(QString filename,int mode)
{
    bool result = false;
    if(mode==0){
        beginResetModel();
        result = mymodel->readFile(filename);
        endResetModel();
    }
    else if(mode == 1){
        beginResetModel();
        mymodel->cleanup();
        result = mymodel->readFile(filename);
        endResetModel();
    }
    return result;

}

void MyFilterModel::addTopLevelCategory()
{
    mymodel->addTopLevelCategory();
}

void MyFilterModel::setFilterStatus(bool status)
{
    mymodel->setFilterStatus(status);
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
bool MyFilterModel::hasToBeDisplayedCat(const QModelIndex index) const
{

//    bool result = true;
//        // How many child this element have


//    QList<QModelIndex> list1;
//    list1<<sourceModel()->index(index.row(), 0, index.parent())<<sourceModel()->index(index.row(), 1, index.parent())
//        <<sourceModel()->index(index.row(), 2, index.parent());
//    QList<QString> type1;
//    type1<<sourceModel()->data(list1[0],Qt::DisplayRole).toString()<<sourceModel()->data(list1[1], Qt::DisplayRole).toString()
//            <<sourceModel()->data(list1[2], Qt::DisplayRole).toString();

//    //QRegularExpression re;
//    QRegularExpression re;

//    for(int i = 0;i<3;i++){
//        re.setPattern(datalist[i]);
//        //qDebug()<<re;
//        if(!type1[i].contains(re)){
//            if(sourceModel()->rowCount(index)>0){
//                for( int ii = 0; ii < sourceModel()->rowCount(index); ii++)
//                {
//                    QModelIndex childIndex = sourceModel()->index(ii,0,index);
//                    if ( ! childIndex.isValid() )
//                        break;
//                    result = hasToBeDisplayed(childIndex);
//                    if (result)
//                    {
//                        // there is atless one element to display
//                        break;
//                    }
//                }

//                return result;
//            }
//        }
//    }

//    return result;

        //bool result = true;
            // How many child this element have

    bool result = true;

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
            result = false;
            if (sourceModel()->parent(index).isValid())
            {
                result = hasToBeDisplayedCat(index.parent());
            }
            return result;
        }

    }
    return result;



}

