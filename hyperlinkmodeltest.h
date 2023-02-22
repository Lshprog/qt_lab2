#ifndef HYPERLINKMODELTEST_H
#define HYPERLINKMODELTEST_H

#include <QObject>
#include <QTest>
#include <QAbstractItemModelTester>
#include "hyperlinkmodel.h"

class HyperlinkModelTest : public QObject
{
    Q_OBJECT
public:
    explicit HyperlinkModelTest(QObject *parent = nullptr);

private:
    bool myCondition();
private slots:
    void initTestCase();

    void myFirstTest();

    void mySecondTest();

    void cleanupTestCase();

};

#endif // HYPERLINKMODELTEST_H
