#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include "dialog.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    HyperlinkDelegate *hyplinkdelegate = new HyperlinkDelegate(this);
    newproxymodel = new MyFilterModel(this);

    ui->treeView->setItemDelegate(hyplinkdelegate);
    ui->treeView->setModel(newproxymodel);
    QFont font  = QFont("Times New Roman",12);
    ui->treeView->setFont(font);
    ui->actionAdd_hyperlink->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionRead_file_triggered()
{
    QString filename= QFileDialog::getOpenFileName(this,"Open file: ");

    newproxymodel->readFile(filename,0);

}


void MainWindow::on_actionShow_info_triggered()
{

}

void MainWindow::on_actionOpen_file_triggered()
{
    QString filename= QFileDialog::getOpenFileName(this,"Open file: ");

    newproxymodel->readFile(filename,1);

}




void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    if(ui->treeView->selectionModel()->currentIndex()==QModelIndex())
        ui->actionAdd_hyperlink->setDisabled(true);
    else{
        ui->actionAdd_category->setDisabled(false);
        ui->actionAdd_hyperlink->setDisabled(false);

        bool status = newproxymodel->checkCategoryStatus(index);

        if(status == false){
            ui->actionAdd_category->setDisabled(true);
            ui->actionAdd_hyperlink->setDisabled(true);
        }

}
}


void MainWindow::on_actionAdd_category_triggered()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();

    QModelIndex result = newproxymodel->addCategory(index);

    if(result!=QModelIndex()){
      ui->treeView->selectionModel()->setCurrentIndex(result,QItemSelectionModel::Select);
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Duplicate!");
        //msgBox.setInformativeText();
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        int ret = msgBox.exec();
    }
}





void MainWindow::on_actionAdd_hyperlink_triggered()
{
    QModelIndex index = ui->treeView->selectionModel()->currentIndex();

    QModelIndex result = newproxymodel->addHyperlink(index);

    if(result!=QModelIndex()){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Duplicate!");
        //msgBox.setInformativeText();
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        int ret = msgBox.exec();
        ui->treeView->selectionModel()->setCurrentIndex(result,QItemSelectionModel::Select);
    }

}


void MainWindow::on_actionDelete_row_triggered()
{

    QModelIndex index = ui->treeView->selectionModel()->currentIndex();

    newproxymodel->removeRows(index.row(),1,index.parent());


}


void MainWindow::on_actionSave_list_of_hyperlinks_triggered()
{
    QString filename= QFileDialog::getSaveFileName(this, "Save As");

        if (filename.isEmpty())
            return;

        QFile file(filename);

        //Open the file
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);

        QList<QString> hyperlinks;
        newproxymodel->makelisthypelinks(&hyperlinks);

        for(int i = 1;i<hyperlinks.size();i++){
            out<<hyperlinks[i].toUtf8()<<"\n";
        }


        file.close();


}

void MainWindow::on_actionAdd_top_level_category_triggered()
{

    QModelIndex result = newproxymodel->addCategory(QModelIndex());

    if(result!=QModelIndex()){
        ui->treeView->selectionModel()->setCurrentIndex(result,QItemSelectionModel::Select);
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Duplicate!");
        //msgBox.setInformativeText();
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        int ret = msgBox.exec();
    }
}


void MainWindow::on_lineEditName_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    newproxymodel->datalist[0] = arg1;
    newproxymodel->setFilterRegularExpression(ui->lineEditName->text());
    ui->treeView->expandAll();
}


void MainWindow::on_lineEditHyperlink_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    newproxymodel->datalist[1] = arg1;
    newproxymodel->setFilterRegularExpression(ui->lineEditHyperlink->text());
    ui->treeView->expandAll();
}


void MainWindow::on_lineEditDescription_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    newproxymodel->datalist[2] = arg1;
    newproxymodel->setFilterRegularExpression(ui->lineEditDescription->text());
    ui->treeView->expandAll();
}


void MainWindow::on_actionSave_file_triggered()
{

    QString filename= QFileDialog::getSaveFileName(this, "Save As");

        if (filename.isEmpty())
            return;

        QFile file(filename);

        //Open the file
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);

        QList<QString> hyperlinks;
        newproxymodel->makeFileInfo(&hyperlinks);

        for(int i = 0;i<hyperlinks.size();i++){
            out<<hyperlinks[i].toUtf8()<<"\n";
        }


        file.close();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    qDebug()<<arg1;
    if(arg1 == 2){
        ui->lineEditHyperlink->setEnabled(false);
        ui->label_5->setEnabled(false);
        newproxymodel->setFilterStatus(true);
    }
    else{
        ui->lineEditHyperlink->setEnabled(true);
        ui->label_5->setEnabled(true);
        newproxymodel->setFilterStatus(false);
    }
}

