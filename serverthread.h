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
#include <vector>
#include "mysqlconn.h"
#include <QTextCodec>

class ServerThread : public QThread{

    Q_OBJECT

public:
    ServerThread(int socketDescriptor, QObject *parent, std::shared_ptr<MySqlConn>);
    ~ServerThread(){}
    void run() override;    
    void packetCreator(char *_buf, QByteArray array, int size);


signals:
    void error(QTcpSocket::SocketError socketError);    
    void boardDisconnected();

    // when a thread has gave all packets to server, the server maks it as done
    void processDoneSignal();

public slots:
    void disconnectionSlot(){
        qDebug() << "Connection lost\n" << endl;
    }

private:
    int socketDescriptor;
    std::shared_ptr<MySqlConn> connection;
};

#endif // SERVERTHREAD_H
