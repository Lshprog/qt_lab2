#ifndef MYFILTERMODEL_H
#define MYFILTERMODEL_H

#include <QSortFilterProxyModel>
#include "hyperlinkmodel.h"

class MyFilterModel : public QSortFilterProxyModel
{
public:
    QList<QString> datalist;


    explicit MyFilterModel(QObject *parent = nullptr);
    virtual bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const override;
    bool addHyperlink(const QModelIndex &index);
    void makelisthypelinks(QList<QString> *list);
    bool addCategory(const QModelIndex &index);
    bool editData(const QModelIndex &index);
    bool checkCategoryStatus(const QModelIndex &index);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool makeFileInfo(QList<QString> *list);
    void readFile(QString filename,int mode);

private:
    bool hasToBeDisplayed(const QModelIndex index) const;
    HyperlinkModel* mymodel;


};

#endif // MYFILTERMODEL_H
