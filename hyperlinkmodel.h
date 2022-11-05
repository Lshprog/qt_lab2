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

private:
    //void showInfo();
    void readFile();
    QStringList getInfo(QString lineString);
    Hyperlink *rootHyperlink;

};

#endif // HYPERLINKMODEL_H
