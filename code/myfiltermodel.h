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
    QModelIndex addHyperlink(const QModelIndex &index);
    void makelisthypelinks(QList<QString> *list);
    QModelIndex addCategory(const QModelIndex &index);
    bool editData(const QModelIndex &index);
    bool checkCategoryStatus(const QModelIndex &index); 
    bool setData(const QModelIndex &index, const QVariant &value, int role)override;
    bool makeFileInfo(QList<QString> *list);
    bool readFile(QString filename,int mode);
    void addTopLevelCategory();
    void setFilterStatus(bool status);
    bool checkTopLevelChildren();
    bool checkIndexValue(const QModelIndex index) const;
    void saveInfoToFile(QString filename);


    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
//    //bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    Qt::DropActions supportedDropActions() const override;
    Qt::DropActions supportedDragActions() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const;


private:

    bool hasToBeDisplayed(const QModelIndex index) const;
    bool hasToBeDisplayedCat(const QModelIndex index) const;
    HyperlinkModel* mymodel;



};

#endif // MYFILTERMODEL_H
