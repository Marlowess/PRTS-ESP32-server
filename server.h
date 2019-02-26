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

    // ~Server(){}

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    int port; // server port listening on
    int scheduledBoards;

private slots:
    void threadFinished();

signals:
    void newConnect();
    void paintDevicesSignal();    

};

#endif // SERVER_H
