#include "tcpserver.h"
#include <QDebug>
#include <QCoreApplication>

tcpserver::~tcpserver()
{
    mTcpServer->close();
}
tcpserver::tcpserver(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &tcpserver::slotNewConnection);
    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is off";
    } else {
        qDebug() << "server is on";
    }
}

void tcpserver::slotNewConnection(){

    if (liveclients >= 6) {
        return;
    }
    QTcpSocket * curr_Socket;
    curr_Socket = mTcpServer->nextPendingConnection();
    connect(curr_Socket, &QTcpSocket::readyRead, this, &tcpserver::slotServerRead);
    connect(curr_Socket, &QTcpSocket::disconnected, this, &tcpserver::slotClientDisconnected);
    liveclients++;
    mTcpSocket.append(curr_Socket);
}


void tcpserver::slotServerRead(){


    QByteArray massive;
    QString clientask,serveranswer;
    int k=0;
    QTcpSocket *curr_Socket = (QTcpSocket*)sender();
    while(curr_Socket->bytesAvailable()>0)
    {

        massive = curr_Socket->readAll();
        clientask.append(massive);
    }
    massive.clear();
    for(int i = 0; i<mTcpSocket.size();i++){
        if(curr_Socket == mTcpSocket.at(i)){
           break;
        }
        k++;
    }
    serveranswer = clientask.trimmed();
    for (int i = 2; i < clientask.size();i++) {
        if((i+1)%3==0)
        {
            serveranswer.append(clientask.at(i));
        }
    }
    sendToClient(k, serveranswer + "\n\r");

}

void tcpserver::slotClientDisconnected(){
    QTcpSocket * disSoc = (QTcpSocket*)sender();
        for(int i = 0; i<mTcpSocket.size();i++){
            if(disSoc == mTcpSocket.at(i)){
                mTcpSocket.removeAt(i);
                liveclients--;
                break;
            }
        }
}
void tcpserver::sendToClient(int pos,QString otvet)
{
    for(int i = 0; i<mTcpSocket.size();i++)
    {
        if (i != pos){
            mTcpSocket[i]->write(otvet.toUtf8());
        }

    }

}
