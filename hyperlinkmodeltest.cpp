#include "hyperlinkmodeltest.h"

HyperlinkModelTest::HyperlinkModelTest(QObject *parent)
    : QObject{parent}
{

}

bool HyperlinkModelTest::myCondition()
{
    return true;
}

void HyperlinkModelTest::initTestCase()
{
    qDebug("Called before everything else.");
}

void HyperlinkModelTest::myFirstTest()
{
    HyperlinkModel *mymodel = new HyperlinkModel();
    auto mymodeltotest = QAbstractItemModelTester(mymodel);
}

void HyperlinkModelTest::mySecondTest()
{
    QVERIFY(myCondition());
    QVERIFY(1 != 2);
}

void HyperlinkModelTest::cleanupTestCase()
{
    qDebug("Called after myFirstTest and mySecondTest.");
}
