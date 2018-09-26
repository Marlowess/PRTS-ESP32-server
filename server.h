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
    int active_threads;
    int syncronizedBoards;

    /* The array will contain packet received by listening thread. The mutex is used to write
       into vector in thread-safe way.
    */
    std::vector<std::string> *packetsArray;
    std::mutex *arrayMutex;


    // Mettere tra gli attributi anche uno shared pointer incapsulante un oggetto che ingloba una lista/mappa
    // di pacchetti.
    // Tale smart pointer sarà passato a ciascun thread figlio, il quale inserirà i vari pacchetti man mano che
    // saranno ricevuti dalle board. La classe che ingloba la struttura deve gestire la concorrenza per
    // garantire l'accesso concorrento alla lista/mappa.
    // In alternativa, proteggere un oggetto DB all'interno dello shared pointer e passare una copia
    // ad ogni thread

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
