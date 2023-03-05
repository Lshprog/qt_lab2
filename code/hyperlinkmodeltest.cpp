#include "hyperlinkmodeltest.h"
#include "qmimedata.h"

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
    mymodel = new HyperlinkModel();
    qDebug("Called before everything else.");
}

void HyperlinkModelTest::modelReadFileTest()
{
    //auto mymodeltotest = QAbstractItemModelTester(mymodel);
    //mymodel->readFile("D:/useless/datatemp.txt");

    QVERIFY(!mymodel->readFile("D:/useless/somename.txt"));
    mymodel->readFile("D:/useless/datatemp.txt");
    Hyperlink* curroot = mymodel->returnroot();
    QVariant data = "Qt";
    QCOMPARE(data,curroot->child(0)->data(0));

}

void HyperlinkModelTest::modelSetDataTest()
{
    QModelIndex index = mymodel->index(0,0,QModelIndex());
    //Hyperlink* curhyp = mymodel->getHyperlinkFromIndex(index);
    //qDebug()<<curhyp->data(0)<<" "<<curhyp->data(1)<<" "<<curhyp->data(2);
    QVariant data = "QTTT";
    mymodel->setData(index,data,Qt::EditRole);
    //qDebug()<<index.column();
    //qDebug()<<data.toString();
    //qDebug()<<mymodel->data(index,Qt::DisplayRole).toString();
    QCOMPARE(data,mymodel->data(index,Qt::DisplayRole));
}

void HyperlinkModelTest::newChildrenTest()
{
    QModelIndex index = mymodel->index(0,0,QModelIndex());
    //Hyperlink* curhyp = mymodel->getHyperlinkFromIndex(index);
    //qDebug()<<curhyp->data(0)<<" "<<curhyp->data(1)<<" "<<curhyp->data(2);
    QList<QVariant> data;
    data<<"Superstar"<<"Game changing"<<"Next lvl me";
    Hyperlink* newh = new Hyperlink(data);
    QVERIFY(mymodel->insertnewrowchild(mymodel->getHyperlinkFromIndex(index)->getChildrenSize(),index,newh));
    QCOMPARE(data[0],mymodel->getHyperlinkFromIndex(index)->child(mymodel->getHyperlinkFromIndex(index)->getChildrenSize()-1)->data(0));
    QCOMPARE(data[1],mymodel->getHyperlinkFromIndex(index)->child(mymodel->getHyperlinkFromIndex(index)->getChildrenSize()-1)->data(1));
    QCOMPARE(data[2],mymodel->getHyperlinkFromIndex(index)->child(mymodel->rowCount(index)-1)->data(2));

    //qDebug()<<index.column();
    //qDebug()<<data.toString();
    //qDebug()<<mymodel->data(index,Qt::DisplayRole).toString();

}

void HyperlinkModelTest::removeRowsTest()
{
    QModelIndex index = mymodel->index(0,0,QModelIndex());
    //qDebug()<<mymodel->getHyperlinkFromIndex(QModelIndex())->child(1)->data(0);
    QVERIFY(mymodel->removeRows(1,1,index));
}

void HyperlinkModelTest::mimeDataTest()
{
    QModelIndexList indexes = QModelIndexList();
    indexes.append(mymodel->index(1,0,mymodel->index(0,0,QModelIndex())));
    QMimeData* mimedata = mymodel->mimeData(indexes);
    Qt::DropAction action;
    QVERIFY(mymodel->dropMimeData(mimedata,action,0,0,mymodel->index(0,0,QModelIndex())));
}

void HyperlinkModelTest::cleanupTestCase()
{
    mymodel->cleanup();
    qDebug("Called after myFirstTest and mySecondTest.");
}
