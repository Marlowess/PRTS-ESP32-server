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
    ~Server(){}

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    int port; // server port listening on
    int number_of_hosts; // number of working boards, setted by user throught GUI or config. file
    int active_threads;

private slots:
    void threadFinished();

signals:
    void newConnect();

};

#endif // SERVER_H
