#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include "dialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QMimeData>
#include <QMetaType>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int id = qRegisterMetaType<Hyperlink>("Hyperlink");

    HyperlinkDelegate *hyplinkdelegate = new HyperlinkDelegate(this);
    newproxymodel = new MyFilterModel(this);

    ui->treeView->setItemDelegate(hyplinkdelegate);
    ui->treeView->setModel(newproxymodel);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //ui->treeView->setDragDropMode(QAbstractItemView::InternalMove);
    ui->treeView->setDragEnabled(true);
    ui->treeView->setAcceptDrops(true);
    ui->treeView->setDropIndicatorShown(true);
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

    if(!filename.isEmpty()){
        newproxymodel->saveInfoToFile("C:/Users/onisa/source/repos/qt_project_2/qt-lab2-lastversion/data/ForDelete.txt");
        if(!newproxymodel->readFile(filename,0)){
            newproxymodel->readFile("C:/Users/onisa/source/repos/qt_project_2/qt-lab2-lastversion/data/ForDelete.txt",1);
        }
    }


}


void MainWindow::on_actionOpen_file_triggered()
{
    QString filename= QFileDialog::getOpenFileName(this,"Open file: ");

    if(!filename.isEmpty()){
        newproxymodel->saveInfoToFile("C:/Users/onisa/source/repos/qt_project_2/qt-lab2-lastversion/data/ForDelete.txt");
        if(!newproxymodel->readFile(filename,1)){
            newproxymodel->readFile("C:/Users/onisa/source/repos/qt_project_2/qt-lab2-lastversion/data/ForDelete.txt",1);
        }
    }

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

    QModelIndex index2 = ui->treeView->selectionModel()->currentIndex();
    //newproxymodel->checkCategoryStatus(index2)
//    if(index2.isValid()){
//        if(newproxymodel->checkCategoryStatus(index2))
//            ui->actionAdd_hyperlink->setEnabled(true);
//         ui->actionAdd_hyperlink->setEnabled(false);
//    }

    if(newproxymodel->checkTopLevelChildren()){
        ui->actionAdd_hyperlink->setEnabled(true);
    }
    ui->actionAdd_hyperlink->setEnabled(false);

    if(index2!=QModelIndex()){
        if(newproxymodel->checkCategoryStatus(index2)){
            ui->actionAdd_hyperlink->setEnabled(true);
            ui->actionAdd_category->setEnabled(true);
        }
    }

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

        for(int i = 0;i<hyperlinks.size();i++){
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(newproxymodel->checkTopLevelChildren()){
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Exit",
                                                                        tr("Do you want to save your file?\n"),
                                                                        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                        QMessageBox::Yes);
            if (resBtn == QMessageBox::Cancel) {
                event->ignore();
            } else if(resBtn == QMessageBox::Yes) {
                on_actionSave_file_triggered();
                event->accept();
            }
            else{
                event->accept();
            }
    }
}

//void MainWindow::dragEnterEvent(QDragEnterEvent *event)
//{
//    if (event->mimeData()->hasFormat("application/x-hyperlink"))
//           event->acceptProposedAction();
//    qDebug()<<"HereNow";
//}



