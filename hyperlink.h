#ifndef HYPERLINK_H
#define HYPERLINK_H
#include <QList>
#include <QString>
#include <QDebug>


class Hyperlink
{
public:
    Hyperlink(const QString name,const QString hyperlink, const QString description,bool category, Hyperlink* parent = nullptr);
    ~Hyperlink();
    void appendChild(Hyperlink* child);
    Hyperlink* child(int row);
    int childCount() const;
    QVariant data(int column) const;
    int row() const;
    Hyperlink *parentHyperlink();
    void setCategoryStatus(bool status);
    bool getCategoryStatus();

    void showinfo();



private:
    QList<Hyperlink* > children;
    QString name;
    QString hyperlink;
    QString description;
    bool category;
    Hyperlink *parent;
};

#endif // HYPERLINK_H
