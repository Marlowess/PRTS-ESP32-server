#include "server.h"

/** Constructor **/
Server::Server(QObject *parent) : QTcpServer(parent){

}

/** Sets server port **/
void Server::setPort(int port){
    this->port = port;
}

/** This method set a new connection, listening on port passed as parameter **/
void Server::setConnection(){
    if (!listen(QHostAddress::Any, port)) {
        qDebug("Unable to start server\n");
        close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

    qDebug("The server is running\n");
}

/** Returns the system time **/
unsigned long Server::getSystemTime(){
    double now = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    unsigned long _now = htonl((unsigned long) now);
    return _now;
}

/** Invoked when a new connection is available **/
void Server::incomingConnection(qintptr socketDescriptor){
    qDebug("New Connection!");
    emit newConnect();
}
