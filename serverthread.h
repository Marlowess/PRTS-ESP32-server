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

class ServerThread : public QThread{

    Q_OBJECT

public:
    ServerThread(int socketDescriptor, QObject *parent, std::mutex *parentMutex, std::vector<std::string> *v,
                 std::shared_ptr<std::condition_variable> cv, std::shared_ptr<std::mutex> cv_mutex,
                 std::shared_ptr<bool> spuriusFlag, std::shared_ptr<bool> isSyncroTime);
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
    //void boardReadySignalChild();
    void boardDisconnected();

    // when a thread has give all packets to server, the server maks it as done
    void processDoneSignal();

public slots:
//    void boardReadySlotChild(){
//        qDebug() << "Authorized to start capture" << endl;
//        // Questo flag viene settato solo qui, perchè verrà utilizzato dal thread per escludere notifiche spurie
//        // in uscita dalla condition variable
//        this->flag = true;
//        sleep(2);
//        (*cv).notify_all();
//    }
    void disconnectionSlot(){
        // Segnalo la disconnessione al padre solo se si tratta di una board attesa per i pacchetti catturati
        // Una board che si scollega quando attendeva il timestamp per entrare nei dispositivi di cattura
        // non viene registrata dal sistema
        if(!firstLap)
            emit boardDisconnected();
        this->exit();
    }

private:
    int socketDescriptor;
    std::shared_ptr<std::condition_variable> cv;
    std::shared_ptr<std::mutex> cv_mutex;    

    std::vector<std::string> *vector; // array where to put packets
    std::mutex *vector_mutex;
    bool firstLap = true; // indica se
    std::shared_ptr<bool> spuriusFlag;
    std::shared_ptr<bool> firstLaunch;
    std::shared_ptr<bool> isSyncroTime;

};

#endif // SERVERTHREAD_H
