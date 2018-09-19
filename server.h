#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QMainWindow>
#include <QtNetwork>
#include <chrono>
#include <arpa/inet.h>

class Server : public QTcpServer{
    Q_OBJECT

public:
    Server(QObject *parent = 0);
    void setPort(int);
    void setConnection();
    unsigned long getSystemTime();
    int sendData(int socket);
    ~Server(){}

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    int port; // server port listening on

signals:
    void newConnect();

};

#endif // SERVER_H
