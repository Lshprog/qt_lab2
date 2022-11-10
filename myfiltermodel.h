#ifndef MYFILTERMODEL_H
#define MYFILTERMODEL_H

#include <QSortFilterProxyModel>

class MyFilterModel : public QSortFilterProxyModel
{
public:
    explicit MyFilterModel(QObject *parent = nullptr);
    virtual bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const override;

private:
    bool hasToBeDisplayed(const QModelIndex index) const;
};

#endif // MYFILTERMODEL_H
