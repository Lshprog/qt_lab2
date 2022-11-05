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





