#include "server.h"
#include "mysqlconn.h"

/** Constructor **/
Server::Server(QObject *parent) : QTcpServer(parent), active_threads(0), syncronizedBoards(0){
    arrayMutex = new std::mutex();
    packetsArray = new std::vector<std::string>();
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

bool Server::allBoardSyncro(){
    return syncronizedBoards == 0;
}

/** Invoked when a new connection is available **/
void Server::incomingConnection(qintptr socketDescriptor){
    qDebug("New Connection!");
    emit newConnect();

    ServerThread *thread = new ServerThread(socketDescriptor, this, arrayMutex, packetsArray);
    connect(thread, &ServerThread::finished, thread, &ServerThread::deleteLater);
    connect(thread, &ServerThread::finished, this, &Server::threadFinished);

    connect(thread, &ServerThread::boardReadySignalChild, this, &Server::boardReadySlotFather);
    connect(this, &Server::boardReadySignalFather, thread, &ServerThread::boardReadySlotChild);

    newThreadRecord();
    thread->start();
    //this->active_threads += 1; // incremento il conteggio dei thread figli

}


/** This method is invoked when a sub-thread ends. If the number of threads becomes zero
 *  means that all boards have finished to exchange data with server.
**/
void Server::threadFinished(){
    qDebug("Thread finished\n");
    this->active_threads -= 1;
    if(active_threads == 0){
        // Quando il numero di thread scende a zero significa che tutte le board hanno iniziato la fase di cattura
        // dei pacchetti. In questa fase di attesa il server può svolgere i vari controlli sui pacchetti, evitando
        // quindi di farlo durante lo scambio di dati, quando il carico di lavoro è già alto
        qDebug() << "All thread have finished" << endl;

        MySqlConn conn;

        if(!conn.openConn("probe_requests_db", "root", "password", "localhost"))
            qDebug() << "Error on creating connection" << endl;
        else
            qDebug() << "OK connection" << endl;

        for(int i = 0; i < packetsArray->size(); i++){
            qDebug() << packetsArray->at(i).c_str() << endl;
            std::vector<std::string> v = split(packetsArray->at(i), ',');
            std::string pHash = v.at(v.size() - 1); // hash received by board
            qDebug() << "Hash" << pHash.c_str() << endl;

            std::size_t found = packetsArray->at(i).find(pHash);
            if (found != std::string::npos){
                qDebug() << "Hash checking OK: they're the same string" << endl;

                // INSERIRE QUI LA CHIAMATA A FUNZIONE PER INSERIRE IL PACCHETTO NEL DB

            }
            else
                qDebug() << "Hash checking FAILED: they're not the same string" << endl;

        }

        // Alla fine svuoto il vector per ricevere i prossimi pacchetti
        packetsArray->clear();
    }
}
