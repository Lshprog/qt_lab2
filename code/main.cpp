#include "mainwindow.h"

#include <QApplication>
#include <hyperlinkmodeltest.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


#ifdef QT_DEBUG
    HyperlinkModelTest test1;
    QTest::qExec(&test1);
#else
    MainWindow w;
    w.show();
#endif
    return a.exec();
}

