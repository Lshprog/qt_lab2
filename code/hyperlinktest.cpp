#include "hyperlinktest.h"

hyperlinktest::hyperlinktest(QObject *parent)
    : QObject{parent}
{

}

bool hyperlinktest::myCondition()
{
    return true;
}

void hyperlinktest::initTestCase()
{
    qDebug("Called before everything else.");
}

void hyperlinktest::myFirstTest()
{
    QVERIFY(true); // check that a condition is satisfied
    QCOMPARE(1, 1); // compare two values
}

void hyperlinktest::mySecondTest()
{
    QVERIFY(myCondition());
    QVERIFY(1 != 2);
}

void hyperlinktest::cleanupTestCase()
{

    qDebug("Called after myFirstTest and mySecondTest.");
}
