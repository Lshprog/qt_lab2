
#ifndef HYPERLINK_H
#define HYPERLINK_H
#include <QVariant>
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

    int getNewRow(bool status);
    int row() const;
    Hyperlink *parentHyperlink();
    void setParentHyperlink(Hyperlink* parent);
    void removechild(int row);

    void setCategoryStatus(bool status);
    bool getCategoryStatus();
    bool getCategoryStatusOfChild(int row);
    int getChildrenSize();

    void insertCategory(Hyperlink* child);

    int columnCount() const;
    bool insertChildren(int position,int count, int columns);
    bool removeChildren(int position, int count);
    int childNumber() const;

    void writeListOfLinks(QList<QString> *list);
    void listInfo(QList<QString> *list,int tabcounter);

    void showInfo();
    bool insertChild(int position, Hyperlink *child);

    int checkDuplicates(Hyperlink *data);



private:
    QList<Hyperlink* > children;
//    QString name;
//    QString hyperlink;
//    QString description;
    QVector<QVariant> columnFields;
    bool category;
    Hyperlink *parent;
};
Q_DECLARE_METATYPE(Hyperlink)

//QDataStream &operator<<(QDataStream &out, Hyperlink &a)
//{

//    out<<a.data(0).toString()<<a.data(1).toString()<<a.data(2).toString();
//    return out;
//}

//QDataStream &operator>>(QDataStream &in, Hyperlink &a)
//{
//    QString cur_string;
//    for(int i =0;i<a.columnCount();i++){
//        in>>cur_string;
//        a.setData(i,cur_string);
//    }

//}


#endif // HYPERLINK_H
