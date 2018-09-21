#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <utility>
#include <chrono>
#include <arpa/inet.h>
#include <QDebug>
#include <string>
#include <cstdio>

class ServerThread : public QThread{

    Q_OBJECT

public:
    ServerThread(int socketDescriptor, QObject *parent);
    ~ServerThread(){}
    void run() override;
    bool sendData(QByteArray data);
    void printTest(){
        qDebug("Nuovi dati da leggere");
    }
    unsigned long getSystemTime();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor;
};

#endif // SERVERTHREAD_H
