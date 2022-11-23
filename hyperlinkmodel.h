#ifndef HYPERLINKMODEL_H
#define HYPERLINKMODEL_H
#include <QSortFilterProxyModel>
#include <QAbstractItemModel>
#include "hyperlink.h"
#include <QFile>
#include <QPair>
#include "dialog.h"
#include "dialogcat.h"

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
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    bool insertRow(int row, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertCategory(int row,const QModelIndex &index);
    Hyperlink *getHyperlinkFromIndex(const QModelIndex &index) const;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    void makelisthypelinks(QList<QString> *list);
    QModelIndex addInfoFromDialog(const QModelIndex &index,Hyperlink *parent);
    QModelIndex addInfoFromDialogCat(const QModelIndex &index,Hyperlink *parent);
    bool makeListInfo(QList<QString> *list);
    bool insertnewrowchild(int row, const QModelIndex &parent, Hyperlink *link);
    bool readFile(QString filename);
    void cleanup();
    void addTopLevelCategory();

    bool getFilterStatus();
    void setFilterStatus(bool status);

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    //bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    Qt::DropActions supportedDropActions() const override;
    Qt::DropActions supportedDragActions() const override;

    Hyperlink* returnroot() const;

    void removeHyperlink(Hyperlink* link);

    bool copyNodes(const QModelIndex& link1,const QModelIndex& link2);

private:
    //void showInfo();
    //bool hasToBeDisplayed(const QModelIndex index) const;
    bool filterStatus;
    QPair<QVector<QVariant>,bool>  getInfo(QString lineString);
    Hyperlink *rootHyperlink;



    // QAbstractItemModel interface


    // QAbstractItemModel interface


    // QAbstractItemModel interface

};

#endif // HYPERLINKMODEL_H
