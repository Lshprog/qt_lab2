#include "mainwindow.h"

#include <QApplication>
#include <hyperlinkmodeltest.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HyperlinkModelTest test1;
    QTest::qExec(&test1);
    MainWindow w;
    w.show();

    return a.exec();
}

