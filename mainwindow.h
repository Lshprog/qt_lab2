#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include "hyperlinkmodel.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionRead_file_triggered();

    void on_actionShow_info_triggered();

    void on_treeView_clicked(const QModelIndex &index);

    void on_actionAdd_row_triggered();

    void on_actionAdd_category_triggered();

private:
    //void readFile();
    //Hyperlink *rootHyperlink;
    //QStringList getInfo(QString lineString);
    Ui::MainWindow *ui;
    HyperlinkModel *mymodel;
    //HyperlinkModel *mymodel;

};
#endif // MAINWINDOW_H
