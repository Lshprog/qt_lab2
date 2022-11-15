#include "hyperlink.h"
#include "qvariant.h"
#include "QMessageBox"


Hyperlink::Hyperlink(const QVector<QVariant> &data, Hyperlink *parent)
{

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
//    for(int i=0;i<children.size();i++){
//        if(children[i]->columnFields[0] == child->columnFields[0] ||children[i]->columnFields[1] == child->columnFields[1]||children[i]->columnFields[1] == child->columnFields[1]){
//            QMessageBox messageBox;
//            messageBox.critical(0,"Error","An error has occured !");
//            messageBox.setFixedSize(500,200);
//            return;
//        }
//    }

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

bool Hyperlink::insertChildren(int position, int count, int columns)
{
    int pos=position;
    if(pos < 0||pos>children.size())
        return false;
    for(int row = 0;row<count;row++){
        QVector<QVariant> data(columns);
        Hyperlink *link = new Hyperlink(data,this);
        this->children.insert(pos,link);
    }

    return true;
}

bool Hyperlink::insertChild(int position, Hyperlink* child)
{
    int pos=position;
    if(pos < 0||pos>children.size())
        return false;

    this->children.insert(pos,child);

    return true;
}

int Hyperlink::checkDuplicates(Hyperlink *data)
{
    for(auto child:this->children){
        if((child->data(0) == data->data(0)&&(!child->data(0).toString().isEmpty()))
                ||(child->data(1) == data->data(1)&&(!child->data(1).toString().isEmpty())))
        {
            return child->row();
        }
    }
    return -1;
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

void Hyperlink::writeListOfLinks(QList<QString> *list)
{
    if(!this->children.empty()){
        //list->append(this->data(1).toString());
        foreach(Hyperlink* child,this->children){
            if(!child->category)
                list->append(child->data(1).toString());
            else
                child->writeListOfLinks(list);
        }
    }
}

void Hyperlink::listInfo(QList<QString> *list,int tabcounter)
{
    if(!this->children.empty()){
        QString cur;
        for(int i=0;i<tabcounter;i++)
            cur=cur+"\t";
        cur =cur+ this->data(0).toString()+"::"+this->data(1).toString()+"::"+this->data(2).toString();
        if(this->getCategoryStatus())
            cur = cur +"::true";
        else
            cur = cur + "::false";
        list->append(cur);
        foreach(Hyperlink *child,this->children)
        {
            //cur = "";
            //for(int i=0;i<tabcounter;i++)
                //cur=cur+"\t";
            //cur =cur+ child->data(0).toString()+"::"+child->data(1).toString()+"::"+child->data(2).toString();
            //list->append(cur);
            tabcounter++;
            child->listInfo(list,tabcounter);
            tabcounter--;
        }
    }
    else{
        QString cur;
        for(int i=0;i<tabcounter;i++)
            cur=cur+"\t";
        cur =cur+ this->data(0).toString()+"::"+this->data(1).toString()+"::"+this->data(2).toString();
        if(this->getCategoryStatus())
            cur = cur +"::true";
        else
            cur = cur + "::false";
        list->append(cur);
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








