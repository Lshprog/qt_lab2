#ifndef HYPERLINK_H
#define HYPERLINK_H
#include <QList>
#include <QString>
#include <QDebug>


class Hyperlink
{
public:
    Hyperlink(const QVector<QVariant> &data, Hyperlink* parent = nullptr);
    ~Hyperlink();
    void appendChild(Hyperlink* child);
    Hyperlink* child(int row);
    int childCount() const;

    QVariant data(int column) const;
    bool setData(int column,const QVariant &value);

    int row() const;
    Hyperlink *parentHyperlink();

    void setCategoryStatus(bool status);
    bool getCategoryStatus();
    bool getCategoryStatusOfChild(int row);
    int getChildrenSize();

    void insertCategory(Hyperlink* child);

    int columnCount() const;
    bool insertChild(int position, Hyperlink* child);
    bool removeChildren(int position, int count);
    int childNumber() const;

    void showInfo();



private:
    QList<Hyperlink* > children;
//    QString name;
//    QString hyperlink;
//    QString description;
    QVector<QVariant> columnFields;
    bool category;
    Hyperlink *parent;
};

#endif // HYPERLINK_H
