#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QMainWindow>
#include <QtNetwork>
#include <QDataStream>
#include <chrono>
#include <arpa/inet.h>
#include "serverthread.h"
#include <vector>
#include <mutex>
#include "utilities.h"
#include <string.h>

class Server : public QTcpServer{
    Q_OBJECT

public:
    Server(QObject *parent = 0);
    void setPort(int);
    void setConnection();
    void setNumberOfHosts(int);
    int getNumberOfHosts(int);
    void newThreadRecord();
    unsigned long getSystemTime();
    bool allBoardSyncro();

    ~Server(){
        delete arrayMutex;
        delete packetsArray;
    }

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    int port; // server port listening on
    int scheduledBoards;
    std::shared_ptr<bool> spuriusFlag;
    bool firstLaunch;
    std::shared_ptr<bool> isSyncroTime;

    /* The array will contain packet received by listening thread. The mutex is used to write
       into vector in thread-safe way.
    */
    std::vector<std::string> *packetsArray;
    std::mutex *arrayMutex;
    std::shared_ptr<std::condition_variable> cv;
    std::shared_ptr<std::mutex> cv_mutex;

private slots:
    void threadFinished();
    void syncroTimeout();
    void sniffingTimeout();

signals:
    void newConnect();
    //void boardReadySignalFather();
    void paintDevicesSignal();    

};

#endif // SERVER_H
