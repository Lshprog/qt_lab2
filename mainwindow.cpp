#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    HyperlinkModel *mymodel = new HyperlinkModel(this);

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
    ui->actionAdd_category->setDisabled(false);
    ui->actionAdd_hyperlink->setDisabled(false);

    Hyperlink *hyperlink = static_cast<Hyperlink*>(index.internalPointer());

    if(hyperlink->getCategoryStatus()==false){
        ui->actionAdd_category->setDisabled(true);
        ui->actionAdd_hyperlink->setDisabled(true);
    }
}

