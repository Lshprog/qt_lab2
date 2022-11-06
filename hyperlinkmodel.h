#ifndef HYPERLINKMODEL_H
#define HYPERLINKMODEL_H

#include <QAbstractItemModel>
#include "hyperlink.h"
#include <QFile>

class HyperlinkModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit HyperlinkModel(QObject *parent = nullptr);
    ~HyperlinkModel() override;

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child)const override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section,Qt::Orientation orientation, int role)const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;




private:
    //void showInfo();
    void readFile();
    QStringList getInfo(QString lineString);
    Hyperlink *rootHyperlink;


    // QAbstractItemModel interface

};

#endif // HYPERLINKMODEL_H
