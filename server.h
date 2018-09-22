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
    int active_threads;
    int syncronizedBoards;


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
