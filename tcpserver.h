#ifndef tcpserver_H
#define tcpserver_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtNetwork>
#include <QByteArray>
#include <QDebug>
#include <QVector>

class tcpserver : public QObject
{
    Q_OBJECT
public:
    explicit tcpserver(QObject *parent = nullptr);
    ~tcpserver();
public slots:
    void slotNewConnection();
    void slotClientDisconnected();
    void sendToClient(int pos,QString otvet);
    void slotServerRead();
private:
    QTcpServer * mTcpServer;
    QVector< QTcpSocket *> mTcpSocket;
    int liveclients = 0;
    //int server_status;
};
#endif // tcpserver_H
