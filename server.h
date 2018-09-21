#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QMainWindow>
#include <QtNetwork>
#include <chrono>
#include <arpa/inet.h>
#include "serverthread.h"

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
    ~Server(){}

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    int port; // server port listening on
    int number_of_hosts; // number of working boards, setted by user throught GUI or config. file
    int syncronizedBoards;
    int active_threads;

private slots:
    void threadFinished();
    void boardReadySlotFather(){
        qDebug() << "Board ready signal received" << endl;
        //syncronizedBoards = 0;
        //syncronizedBoards--;
        if(syncronizedBoards == 0){
            emit boardReadySignalFather();
            // tutti i thread autorizzano le board a catturare pacchetti
        }
    }

signals:
    void newConnect();
    void boardReadySignalFather();

};

#endif // SERVER_H
