#include "serverthread.h"

ServerThread::ServerThread(int socketDescriptor, QObject *parent, std::mutex *m, std::vector<std::string> *v,
                           std::shared_ptr<std::condition_variable> cv, std::shared_ptr<std::mutex> cv_mutex,
                           std::shared_ptr<bool> spuriusFlag)
    :QThread(parent), socketDescriptor(socketDescriptor){
    vector = v;
    this->vector_mutex = m;
    this->cv_mutex = cv_mutex;
    this->cv = cv;
    this->spuriusFlag = spuriusFlag;
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

        if(size == 1){
            dataBuffer = tcpSocket.read(1);
            if(dataBuffer.size() < 1){
                qDebug() << "Error on size" << endl;
                break;
            }

            // Dati di dimensione 1 non possono che corrispondere alla richiesta di sincronizzazione orologio
            // da parte delle board. Questo comando viene inviato in due diverse situazioni:
            // 1) la board viene attivata per la prima volta, e dunque ha bisogno di settare il proprio
            // clock interno prima di cominciare la cattura
            // 2) dopo l'invio dei pacchetti la board richiede l'orologio prima di effettuare una nuova cattura
            if(dataBuffer.toStdString().compare("|") == 0){
                qDebug() << "I'm here" << endl;

                // La board richiede il timestamp. La blocco fino a nuovo ordine
                // wait su condition variable
                std::unique_lock<std::mutex> ul(*(cv_mutex));

                emit processDoneSignal();
                // wait su condition variable. In fase di sblocco si controlla l'eventuale risveglio spurio,
                // e nel caso si torna in fase di attesa su cv
                (*cv).wait(ul, [&](){
                    qDebug() << (*spuriusFlag) << endl;
                    return *(spuriusFlag);
                });

                // Arrivo qui quando il padre sveglia tutti quelli in attesa su condition variable
                unsigned long time = this->getSystemTime();
                qDebug() << "Time: " << time << endl;
                QByteArray writingBuffer(std::to_string(time).c_str());
                int result = tcpSocket.write(writingBuffer);
                if(result == -1 || result < writingBuffer.size()){
                    qDebug() << "Error on sending timestamp" << endl;
                    break;
                }

                result = tcpSocket.write(QByteArray("GO"));
                if(result == -1 || result < 2){
                    qDebug() << "Error on sending start signal" << endl;
                    break;
                }
            }
        }

        // Se i dati hanno una dimensione maggiore di 1, significa che ci troviamo nella fase di trasmissione
        // dei dati tra client e server.
        // TODO: stabilire se il controllo hash debba essere fatto adesso oppure nella successiva fase, quella
        // in cui il thread padre effettua le analisi mentre le board catturano.
        // Tip: le analisi e il salvataggio del DB potrebbe essere svolto tra il momento in cui tutte le board
        // sono sincronizzate (avviene il risveglio dei thread bloccati sulla condition variable) oppure subito
        // dopo.
        else{
            firstLap = false;
            dataBuffer = tcpSocket.read(size);
            if(dataBuffer.size() < size){
                qDebug() << "Error on receiving packet" << endl;
                break;
            }
            char _buf[256];
            packetCreator(_buf, dataBuffer, size);

            /* TODO Implementare una nuova funzione di hash standard, come MD5 */
            //qDebug() << "Hash string: " << hashFunction(std::string(_buf)).c_str();
            qDebug() << "Packet received" << endl;
            (*vector_mutex).lock();
            vector->push_back(std::string(_buf));
            (*vector_mutex).unlock();

            emit processDoneSignal(); // this thread has finished to push data into vector
        }
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



