#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QTcpServer>
#include <QByteArray>
#include <QDataStream>
#include <utility>
#include <chrono>
#include <arpa/inet.h>
#include <QDebug>
#include <string>
#include <cstdio>
#include <condition_variable>
#include <mutex>
#include "server.h"

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
        sleep(2);
        cv.notify_one();
    }
private:
    int socketDescriptor;
    std::condition_variable cv;
    std::mutex m;
    QObject *parent;
};

#endif // SERVERTHREAD_H
