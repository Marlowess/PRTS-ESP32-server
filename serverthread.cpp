#include "serverthread.h"
#include <iostream>

ServerThread::ServerThread(int socketDescriptor, QObject *parent, std::mutex *m, std::vector<std::string> *v,
                           std::shared_ptr<std::condition_variable> cv, std::shared_ptr<std::mutex> cv_mutex,
                           std::shared_ptr<bool> spuriusFlag, std::shared_ptr<bool> isSyncroTime)
    :QThread(parent), socketDescriptor(socketDescriptor){
    vector = v;
    this->vector_mutex = m;
    this->cv_mutex = cv_mutex;
    this->cv = cv;
    this->spuriusFlag = spuriusFlag;
    this->isSyncroTime = isSyncroTime;
}

/** When thread starts, this method is invoked **/
void ServerThread::run(){    
    QTcpSocket tcpSocket;

    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }

    // Se questo socket si disconnette per qualche motivo, viene invocato l'apposito slot
    connect(&tcpSocket, &QTcpSocket::disconnected, this, &ServerThread::disconnectionSlot);

    while(tcpSocket.state() == QTcpSocket::ConnectedState){
        tcpSocket.waitForReadyRead(-1);
        QByteArray buffer = tcpSocket.read(sizeof(int));
        QDataStream ds(buffer);
        int size;
        ds >> size;
        qDebug() << "Size: " << size << endl;

        if(size == 0){
            qDebug() << "No data to read" << endl;
            firstLap = false;
            break;
        }

        if(!tcpSocket.waitForReadyRead(4000))
            break;

        QByteArray dataBuffer;

        // Se i dati hanno una dimensione maggiore di 1, significa che ci troviamo nella fase di trasmissione
        // dei dati tra client e server.
        // TODO: stabilire se il controllo hash debba essere fatto adesso oppure nella successiva fase, quella
        // in cui il thread padre effettua le analisi mentre le board catturano.
        // Tip: le analisi e il salvataggio del DB potrebbe essere svolto tra il momento in cui tutte le board
        // sono sincronizzate (avviene il risveglio dei thread bloccati sulla condition variable) oppure subito
        // dopo.

        dataBuffer = tcpSocket.read(size);
        if(dataBuffer.size() < size){
            qDebug() << "Error on receiving packet" << endl;
            break;
        }
        qDebug() << dataBuffer << endl;
        char _buf[256];
        packetCreator(_buf, dataBuffer, size);
        //qDebug() << dataBuffer << endl;

        /* TODO Implementare una nuova funzione di hash standard, come MD5 */
        //qDebug() << "Hash string: " << hashFunction(std::string(_buf)).c_str();
        qDebug() << "Packet received" << endl;
//        (*vector_mutex).lock();
//        vector->push_back(std::string(_buf));
//        (*vector_mutex).unlock();

        //emit processDoneSignal(); // this thread has finished to push data into vector
    }


    // Arrivati qui la board ha già effettuato la disconnessione
    tcpSocket.disconnectFromHost();
    //tcpSocket.waitForDisconnected();

}

/** Returns the system time **/
unsigned long ServerThread::getSystemTime(){
    double now = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    unsigned long _now = htonl((unsigned long) now);
    return _now;
}

/** Makes the packet structure **/
void ServerThread::packetCreator(char *_buf, QByteArray array, int size){
    int i = 0, j = 0;
    std::string str(array.toStdString());
    str[size] = '\0';
    const char *buf = str.c_str();
    for(i = 0; i < size - 2; i++) {
        if ( buf[i] == '\0') continue;
        _buf[j++] = buf[i];
    }
    _buf[j] = '\0';
}



