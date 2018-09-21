#include "serverthread.h"

ServerThread::ServerThread(int socketDescriptor, QObject *parent)
    :QThread(parent), socketDescriptor(socketDescriptor){}

void ServerThread::run(){
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }
    tcpSocket.waitForReadyRead(-1);

    QByteArray buffer = tcpSocket.read(sizeof(int));
    QDataStream ds(buffer);
    int size;
    ds >> size;
    qDebug() << "Size: " << size << endl;

    tcpSocket.waitForReadyRead(-1);
    QByteArray dataBuffer;
    int toRead = size;
    while(toRead > 0){
        dataBuffer = tcpSocket.read(size);
        qDebug() << "Read: " << dataBuffer.data() << endl;
        toRead -= dataBuffer.size();
    }

    if((size == 1) && (dataBuffer.toStdString().compare("|") == 0)){
        unsigned long time = this->getSystemTime();
        qDebug() << "Time: " << time << endl;
        qDebug() << "Time2: " << std::to_string(time).c_str() << endl;
        //tcpSocket.flush();
        //QByteArray array(std::to_string(time).c_str());
        //tcpSocket.write(array, 10);
    }
    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
}

/** Returns the system time **/
unsigned long ServerThread::getSystemTime(){
    double now = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    unsigned long _now = htonl((unsigned long) now);
    return _now;
}
