#include "myfiltermodel.h"
#include <QQueue>
#include <QFileDialog>

#include <QMimeData>
#include <QCoreApplication>
#include <QMimeType>

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

bool MyFilterModel::checkTopLevelChildren()
{
    Hyperlink *root = mymodel->returnroot();
    if(root->getChildrenSize()>0)
        return true;
    return false;
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
        return checkIndexValue(index);

    }
    return result;

}
bool MyFilterModel::hasToBeDisplayedCat(const QModelIndex index) const
{
    bool result = checkIndexValue(index);

    if(!result){

        QQueue<QModelIndex> queue;
        queue.enqueue(index);
        while(!queue.isEmpty()){
            QModelIndex cur_index = queue.dequeue();

            if(sourceModel()->rowCount(cur_index)>0){
                for(int i =0;i<this->sourceModel()->rowCount(cur_index);i++){
                    Hyperlink *cur_hyp = mymodel->getHyperlinkFromIndex(sourceModel()->index(i,0,cur_index));
                    if(cur_hyp->getCategoryStatus())
                        result = checkIndexValue(sourceModel()->index(i,0,cur_index));
                    if(result){
                        return result;
                    }

                    queue.enqueue(sourceModel()->index(i,0,cur_index));
                }
            }

        }

        QModelIndex cur_parent = index;
        while(cur_parent!=QModelIndex()){
            cur_parent = cur_parent.parent();
            result = checkIndexValue(cur_parent);
            if(result)
               return result;

        }

    }
    return result;
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


   file.close();
}


//static const char s_treeNodeMimeType[] = "application/x-hyperlink";
////static const char s_treeNodeMimeType[] = "application/x-qabstractitemmodeldatalist";

QStringList MyFilterModel::mimeTypes() const
{
    qDebug()<<"here out!!!!!!!!!";
    return mymodel->mimeTypes();
}

QMimeData *MyFilterModel::mimeData(const QModelIndexList &indexes) const
{
    qDebug()<<"here out";
    QModelIndexList indexes2;
    for(const QModelIndex &index : indexes){
        indexes2<<mapToSource(index);
    }

    return mymodel->mimeData(indexes2);
    //return mymodel->mimeData(indexes);
}



bool MyFilterModel::dropMimeData(const QMimeData *mimeData, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    qDebug()<<"NOW HERE out";
    beginResetModel();
    return mymodel->dropMimeData(mimeData,action,row,column,mapToSource(parent));
    endResetModel();
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
   return mymodel->flags(mapToSource(index));
}
