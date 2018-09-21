#include "server.h"

/** Constructor **/
Server::Server(QObject *parent) : QTcpServer(parent), active_threads(0){

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

void Server::setNumberOfHosts(int n){
    this->number_of_hosts = n;
}

int Server::getNumberOfHosts(int){
    return this->number_of_hosts;
}

void Server::newThreadRecord(){
    qDebug("New Thread\n");
    active_threads += 1;
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

    // Mettere qui un controllo sul numero di thread attualmente in vita, confrontandoli con il numero
    // di board che il sistema si aspetta, in base a quando definito dall'utente tramite GUI
    // Se il numero di thread è uguale a quello attesso, il server deve mettersi in attesa dei vari thread
    // tramite join()

    ServerThread *thread = new ServerThread(socketDescriptor, this);
    connect(thread, &ServerThread::finished, thread, &ServerThread::deleteLater);
    connect(thread, &ServerThread::finished, this, &Server::threadFinished);
    newThreadRecord();
    thread->start();

}

void Server::threadFinished(){
    qDebug("Thread finished\n");
    this->active_threads -= 1;
}




