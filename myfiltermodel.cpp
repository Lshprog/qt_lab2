#include "myfiltermodel.h"

MyFilterModel::MyFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{

}

bool MyFilterModel::filterAcceptsRow(int source_row, const QModelIndex & source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    return hasToBeDisplayed(index);
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
            QModelIndex useIndex = sourceModel()->index(index.row(), 0, index.parent());
            QString type = sourceModel()->data(useIndex, Qt::DisplayRole).toString();
            if ( ! type.contains(filterRegularExpression()))
            {
               result = false;
            }
            else
            {
                result = true;
            }
        }
        return result;

}
