#include "hyperlink.h"
#include "qvariant.h"



Hyperlink::Hyperlink(const QString name, const QString hyperlink, const QString description,bool category, Hyperlink *parent)
{
    this->name = name;
    this->hyperlink = hyperlink;
    this->description = description;
    this->category = category;
    this->parent = parent;
}

Hyperlink::~Hyperlink()
{
    qDeleteAll(children);
}

void Hyperlink::appendChild(Hyperlink *child)
{
    children.append(child);
}

Hyperlink *Hyperlink::child(int row)
{
    return children.value(row);
}

int Hyperlink::childCount() const
{
    return children.count();
}

QVariant Hyperlink::data(int column) const
{
    if(column == 0)
        return name;
    if(column == 1)
        return hyperlink;
    if(column == 2)
        return description;
    return QVariant();
}

bool Hyperlink::setData(int column, const QVariant &value)
{
    this->data(column) = value;
}

int Hyperlink::row() const
{
    if(parent){
        parent->children.indexOf(const_cast<Hyperlink*>(this));
    }

    return 0;
}

Hyperlink *Hyperlink::parentHyperlink()
{
    return parent;
}

void Hyperlink::setCategoryStatus(bool status)
{
    this->category = status;
}

bool Hyperlink::getCategoryStatus()
{
    return this->category;
}

void Hyperlink::showinfo()
{
    qDebug()<<"Hyperlink: "<<name<<" "<<hyperlink<<" "<<description<<"("<<childCount()<<"children)";

    foreach (Hyperlink *child, children) {
        child->showinfo();
    }

}








