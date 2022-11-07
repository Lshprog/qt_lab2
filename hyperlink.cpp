#include "hyperlink.h"
#include "qvariant.h"



Hyperlink::Hyperlink(const QVector<QVariant> &data, Hyperlink *parent)
{
//    this->name = name;
//    this->hyperlink = hyperlink;
//    this->description = description;
    this->category = false;
    this->parent = parent;
    columnFields = data;
}

Hyperlink::~Hyperlink()
{
    qDeleteAll(children);
}

void Hyperlink::appendChild(Hyperlink *child)
{
    if(child->getCategoryStatus())
        this->children.insert(this->children.begin(),child);
    else
        this->children.append(child);
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
    return columnFields.value(column);
}

bool Hyperlink::setData(int column, const QVariant &value)
{
    if(column<0 || column > this->columnFields.size())
        return false;

    columnFields[column] = value;

    return true;

}

int Hyperlink::row() const
{
    if(parent){
        return parent->children.indexOf(const_cast<Hyperlink*>(this));
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

bool Hyperlink::getCategoryStatusOfChild(int row)
{
    if(children[row])
        return children[row]->getCategoryStatus();
    return false;
}

int Hyperlink::getChildrenSize()
{
    return children.size();
}

void Hyperlink::insertCategory(Hyperlink *child)
{
    children.push_front(child);
}

int Hyperlink::columnCount() const
{
    return columnFields.count();
}

bool Hyperlink::insertChild(int position, Hyperlink* child)
{
    int pos=position;
    if(pos < 0||pos>children.size())
        return false;

    this->children.insert(pos,child);

    return true;
}

bool Hyperlink::removeChildren(int position, int count)
{
    if(position < 0||position>children.size())
        return false;

    for(int row = 0; row<count;row++){
        Hyperlink *child = children.takeAt(position);
        delete child;
    }

    return true;
}

int Hyperlink::childNumber() const
{
    if(parent){
        return parent->children.indexOf(const_cast<Hyperlink*>(this));
    }

    return 0;
}

bool Hyperlink::writeListOfLinks(QList<QString> *list)
{
    if(!this->children.empty()){
        list->append(this->data(1).toString());
        foreach(Hyperlink* child,this->children){
            if(!child->category)
                list->append(child->data(1).toString());
            else
                child->writeListOfLinks(list);
        }
    }
}


void Hyperlink::showInfo()
{
    qDebug()<<"Person: "<<columnFields.at(0).toString()<<"<"<<childCount()<<"children)";

    QStringList columns;
    foreach(QVariant column, columnFields){
        columns.append(column.toString());
    }
    qDebug()<<"Columns: "<<columns;
    foreach (Hyperlink *child,children){
        child->showInfo();
    }

}








