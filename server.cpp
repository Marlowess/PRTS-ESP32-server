#include "server.h"
#include "mysqlconn.h"

/** Constructor **/
Server::Server(QObject *parent) : QTcpServer(parent){
    arrayMutex = new std::mutex();
    packetsArray = new std::vector<std::string>();
    this->cv = std::make_shared<std::condition_variable>();
    this->cv_mutex = std::make_shared<std::mutex>();
    firstLaunch = true;
    spuriusFlag = std::make_shared<bool>(false);
    isSyncroTime = std::make_shared<bool>(true); // la prima volta che viene lanciato il server, esso accetta solo richieste di timestamp

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

    // Qui metto il timer da 10", che sarà il tempo massimo che le board avranno per richiedere il timestamp
    // connetto lo scadere del timer ad una variabile booleana, la quale controlla cosa il server può accettare
    // in un determinato istante di tempo: richieste di timestamp o pacchetti
    //QTimer::singleShot(5000, this, &Server::syncroTimeout);
}

/** This method sets how many boards are expected by server during syncro. mode **/
void Server::setNumberOfHosts(int n){
    this->scheduledBoards = n;
}

/** It returns number of scheduled boards so far **/
int Server::getNumberOfHosts(int){
    return this->scheduledBoards;
}

void Server::newThreadRecord(){
    qDebug("New Thread\n");
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
    /* The following code is executed each time a new board gets connected to server */
    qDebug("New Connection!");
    emit newConnect();
    ServerThread *thread = new ServerThread(socketDescriptor, this);
    connect(thread, &ServerThread::finished, thread, &ServerThread::deleteLater);
    //connect(thread, &ServerThread::finished, this, &Server::threadFinished);

    newThreadRecord();
    thread->start();
}


/** This method is invoked when a sub-thread ends. If the number of threads becomes zero
 *  means that all boards have finished to exchange data with server.
**/
void Server::threadFinished(){
    qDebug("Thread finished\n");
    qDebug() << "All thread have finished" << endl;

    MySqlConn conn;

    if(!conn.openConn("probe_requests_db", "root", "password", "localhost"))
        qDebug() << "Error on creating connection" << endl;
    else
        qDebug() << "OK connection" << endl;

    for(int i = 0; i < packetsArray->size(); i++){
        //qDebug() << packetsArray->at(i).c_str() << endl;
        std::vector<std::string> v = split(packetsArray->at(i), ',');
        std::string pHash = v.at(v.size() - 1); // hash received by board
        //qDebug() << "Hash" << pHash.c_str() << endl;

        std::size_t found = packetsArray->at(i).find(pHash);
        if (found != std::string::npos){
            //qDebug() << "Hash checking OK: they're the same string" << endl;

            // INSERIRE QUI LA CHIAMATA A FUNZIONE PER INSERIRE IL PACCHETTO NEL DB
            conn.insertData(QString(packetsArray->at(i).c_str()));

        }
        else
            qDebug() << "Hash checking FAILED: they're not the same string" << endl;

    }

    // Alla fine svuoto il vector per ricevere i prossimi pacchetti
    packetsArray->clear();
    conn.selectAll();

    // FARE QUI I VARI FILTRAGGI, SCARTANDO I PACCHETTI CHE NON SERVONO
    // Una volta finito lanciare un segnale alla GUI, che aprirà il DB e disegnerà
    // i puntini sul grafico
    emit paintDevicesSignal();

    // In questo punto devo aggiornare i parametri della cattura che sta per iniziare.
    // Devo aggiornare il numero di board attese per la prossima cattura, azzerare il numero
    // di board che hanno completato il processo
    //this->syncronizedBoards = number_of_hosts; // riporto il numero di board al valore atteso iniziale
    //boardsProcessDone = 0;

    //sleep(5);
    sleep(3);
    std::lock_guard<std::mutex> lg(*(cv_mutex));
    (*spuriusFlag) = true; // flag delle notifiche spurie disattivato
    qDebug() << "notifyAll now!" << endl;
    (*cv).notify_all();
    (*spuriusFlag) = false; // flag delle notifiche spurie riattivato
}

/** This method is invoked when the syncronization mode timer is ended **/
void Server::syncroTimeout(){
    // I've to change the check variable, so that new connections from now are rejected
    /* Una volta che questa variabile diventa false, i thread figli sanno che non devono accettare
       nuove richieste di timestamp, perchè in questa fase il server accetta solo nuove connessioni per
       l'upload di nuovi pacchetti catturati
    */
    qDebug() << "Timestamp exchanging period is over!" << '\n';
    *(isSyncroTime) = false;
    QTimer::singleShot(5000, this, &Server::sniffingTimeout);
}

void Server::sniffingTimeout(){
    qDebug() << "Packets exchanging period is over!" << '\n';
    QTimer::singleShot(5000, this, &Server::syncroTimeout);
}
