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
    int number_of_hosts; // number of working boards, setted by user throught GUI or config. file
    int active_threads; // represents how many threads are active
    int syncronizedBoards; // represents how many boards server waiting for storing data
    int boardsProcessDone; // represents how many boards have talked with server already
    std::shared_ptr<bool> spuriusFlag;
    bool firstLaunch;

    /* The array will contain packet received by listening thread. The mutex is used to write
       into vector in thread-safe way.
    */
    std::vector<std::string> *packetsArray;
    std::mutex *arrayMutex;
    std::shared_ptr<std::condition_variable> cv;
    std::shared_ptr<std::mutex> cv_mutex;

private slots:
    void threadFinished();

    void decrementingBoardsSlot(){
        syncronizedBoards--;
    }

    void incrementBoardsDone(){
        if(firstLaunch){
            if(active_threads == number_of_hosts){
                qDebug() << "I'm in it!" << endl;
                sleep(3);
                std::lock_guard<std::mutex> lg(*(cv_mutex));
                (*spuriusFlag) = true; // flag delle notifiche spurie disattivato
                //sleep(5);
                (*cv).notify_all();
                (*spuriusFlag) = false; // flag delle notifiche spurie riattivato
                firstLaunch = false;
            }
        }
        boardsProcessDone++;
    }

signals:
    void newConnect();
    //void boardReadySignalFather();
    void paintDevicesSignal();

};

#endif // SERVER_H
