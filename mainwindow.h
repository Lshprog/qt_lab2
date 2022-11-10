#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSortFilterProxyModel>
#include <QMainWindow>
#include <QFile>
#include "hyperlinkmodel.h"
#include "hyperlinkdelegate.h"
#include "myfiltermodel.h"


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

    void on_actionAdd_hyperlink_triggered();

    void on_actionAdd_subcategory_triggered();

    void addInfoFromDialog(int endrow,const QModelIndex &index,Hyperlink *parent, bool status);

    void on_actionEdit_row_triggered();

    void on_actionDelete_row_triggered();

    void on_actionSave_list_of_hyperlinks_triggered();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    //void readFile();
    //Hyperlink *rootHyperlink;
    //QStringList getInfo(QString lineString);
    Ui::MainWindow *ui;
    HyperlinkModel *mymodel;
    MyFilterModel *newproxymodel;
    //QSortFilterProxyModel *newproxymodel;
    //HyperlinkModel *mymodel;

};
#endif // MAINWINDOW_H
