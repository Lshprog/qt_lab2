#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include "dialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    mymodel = new HyperlinkModel(this);

    HyperlinkDelegate *hyplinkdelegate = new HyperlinkDelegate(this);


    ui->treeView->setItemDelegateForColumn(1,hyplinkdelegate);
    ui->treeView->setModel(mymodel);

    //ui->treeView->selectionModel()->selectedIndexes()

    //ui->actionAdd_category->setDisabled(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionRead_file_triggered()
{

}


void MainWindow::on_actionShow_info_triggered()
{

}






void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    ui->actionAdd_subcategory->setDisabled(false);
    ui->actionAdd_hyperlink->setDisabled(false);

    Hyperlink *hyperlink = static_cast<Hyperlink*>(index.internalPointer());

    if(hyperlink->getCategoryStatus()==false){
        ui->actionAdd_subcategory->setDisabled(true);
        ui->actionAdd_hyperlink->setDisabled(true);
    }
}


void MainWindow::on_actionAdd_row_triggered()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();

    Hyperlink *childeee = static_cast<Hyperlink*>(index.internalPointer());
    Hyperlink *parentte = childeee->parentHyperlink();
    int endrow = parentte->getChildrenSize();


    addInfoFromDialog(endrow,index.parent(),parentte,false);

}


void MainWindow::on_actionAdd_category_triggered()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();

    Hyperlink *childeee = static_cast<Hyperlink*>(index.internalPointer());
    Hyperlink *parentte = childeee->parentHyperlink();
    int endrow = 0;

    addInfoFromDialog(endrow,index.parent(),parentte,true);
}





void MainWindow::on_actionAdd_hyperlink_triggered()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    Hyperlink *parentte = static_cast<Hyperlink*>(index.internalPointer());
    //parentte->showInfo();
    //Hyperlink *parentte = childeee->parentHyperlink();
    int endrow = parentte->getChildrenSize();

    addInfoFromDialog(endrow,index,parentte,false);

}



void MainWindow::on_actionAdd_subcategory_triggered()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    Hyperlink *parentte = static_cast<Hyperlink*>(index.internalPointer());
    //parentte->showInfo();
    //Hyperlink *parentte = childeee->parentHyperlink();
    int endrow = 0;

    addInfoFromDialog(endrow,index,parentte,true);
}

void MainWindow::addInfoFromDialog(int endrow,const QModelIndex &index, Hyperlink *parent,bool status)
{
    QVector<QVariant> data;

    Dialog win(this);

    win.show();

    if(!win.exec())
        return;

    data<<win.returnName()<<win.returnLink()<<win.returnDescription();

    Hyperlink* new_hyperlink = new Hyperlink(data,parent);
    new_hyperlink->setCategoryStatus(status);

    //mymodel->insertnewrowchild(endrow,index.parent(),new_hyperlink);
    mymodel->insertnewrowchild(endrow,index,new_hyperlink);
}


void MainWindow::on_actionEdit_row_triggered()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    Hyperlink *currenthyp = mymodel->getHyperlinkFromIndex(index);



    Dialog win(this);

    win.show();

    win.setName(currenthyp->data(0).toString());
    win.setLink(currenthyp->data(1).toString());
    win.setDescription(currenthyp->data(2).toString());


    if(!win.exec())
        return;

    QVector<QVariant> data;

    data<<win.returnName()<<win.returnLink()<<win.returnDescription();

    mymodel->setData(mymodel->index(currenthyp->row(),0,index.parent()),data[0],Qt::EditRole);
    mymodel->setData(mymodel->index(currenthyp->row(),1,index.parent()),data[1],Qt::EditRole);
    mymodel->setData(mymodel->index(currenthyp->row(),2,index.parent()),data[2],Qt::EditRole);

}


void MainWindow::on_actionDelete_row_triggered()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();

    mymodel->removeRows(index.row(),1,index.parent());
}

