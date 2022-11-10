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

     enum HyperlinkRoles{CategoryRole=Qt::UserRole,};


    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child)const override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section,Qt::Orientation orientation, int role)const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    bool insertRow(int row, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertCategory(int row,const QModelIndex &index);
    Hyperlink *getHyperlinkFromIndex(const QModelIndex &index) const;

    void makelisthypelinks(QList<QString> *list);

    bool insertnewrowchild(int row, const QModelIndex &parent, Hyperlink *link);

    Hyperlink* returnroot() const;


private:
    //void showInfo();

    void readFile();
    QVector<QVariant> getInfo(QString lineString);
    Hyperlink *rootHyperlink;

};

#endif // HYPERLINKMODEL_H
