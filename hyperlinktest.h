#ifndef HYPERLINKTEST_H
#define HYPERLINKTEST_H
#include <QTest>
#include <QObject>

class hyperlinktest : public QObject
{
    Q_OBJECT
public:
    explicit hyperlinktest(QObject *parent = nullptr);

private:
    bool myCondition();
private slots:
    void initTestCase();

    void myFirstTest();

    void mySecondTest();

    void cleanupTestCase();

};

#endif // HYPERLINKTEST_H
