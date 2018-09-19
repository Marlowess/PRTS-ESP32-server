#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QMainWindow>
#include <QtNetwork>

class Server : public QTcpServer{
    Q_OBJECT

public:
    Server(QObject *parent = 0);
    void setPort(int);
    void setConnection();
    ~Server(){}

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    int port; // server port listening on

signals:
    void newConnect();

};

#endif // SERVER_H
