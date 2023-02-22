#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QObject>

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    MyServer();
};

#endif // MYSERVER_H
