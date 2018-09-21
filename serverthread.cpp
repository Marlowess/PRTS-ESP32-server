#include "serverthread.h"

ServerThread::ServerThread(int socketDescriptor, QObject *parent)
    :QThread(parent), socketDescriptor(socketDescriptor){}

void ServerThread::run(){
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }

    while(tcpSocket.state() == QTcpSocket::ConnectedState){
        tcpSocket.waitForReadyRead(-1);
        QByteArray buffer = tcpSocket.read(sizeof(int));
        QDataStream ds(buffer);
        int size;
        ds >> size;
        qDebug() << "Size: " << size << endl;

        tcpSocket.waitForReadyRead(-1);
        QByteArray dataBuffer;

        if(size == 0){
            qDebug() << "No data to read" << endl;
            break;
        }

        if(size == 1){
            dataBuffer = tcpSocket.read(1);
            if(dataBuffer.size() < 1){
                qDebug() << "Error on size" << endl;
                break;
            }
            if(dataBuffer.toStdString().compare("|") == 0){
                qDebug() << "I'm here" << endl;
                unsigned long time = this->getSystemTime();
                qDebug() << "Time: " << time << endl;
                QByteArray writingBuffer(std::to_string(time).c_str());
                int result = tcpSocket.write(writingBuffer);
                if(result == -1 || result < writingBuffer.size()){
                    qDebug() << "Error on sending timestamp" << endl;
                    break;
                }

                // QUI dovrà essere messa l'attesa su condition variable. Si sbloccherà quando
                // tutte le board saranno state sentite, oppure allo scadere di un determinato timer, se una
                // delle board ha perso la connessione alla rete
                result = tcpSocket.write(QByteArray("GO"));
                if(result == -1 || result < 2){
                    qDebug() << "Error on sending start signal" << endl;
                    break;
                }
            }
        }

        // CASO DI INVIO DI PACCHETTI DA PARTE DEL CLIENT
        else{
            dataBuffer = tcpSocket.read(size);
            if(dataBuffer.size() < size){
                qDebug() << "Error on receiving packet" << endl;
                break;
            }
            char _buf[256];
            packetCreator(_buf, dataBuffer, size);
            qDebug() << "Pacchetto: " << _buf << endl;
        }
    }

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
