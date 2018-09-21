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
    void packetCreator(char *_buf, QByteArray array, int size);

signals:
    void error(QTcpSocket::SocketError socketError);
    void boardReadySignalChild();

public slots:
    void boardReadySlotChild(){
        qDebug() << "Authorized to start capture" << endl;
    }
private:
    int socketDescriptor;
};

#endif // SERVERTHREAD_H
