#include "serverthread.h"
#include <iostream>

ServerThread::ServerThread(int socketDescriptor, QObject *parent, std::shared_ptr<MySqlConn> conn)
    :QThread(parent), socketDescriptor(socketDescriptor)/*, connection(connection)*/{
    unsigned long now = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1000);
    connection.openConn("probe_requests_db", "root", "password", "localhost", QString::number(now) + qrand());
}

/** When thread starts, this method is invoked **/
void ServerThread::run(){    
    QTcpSocket tcpSocket;

    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }

    /* This slot is invoked if the connection gets lost */
    connect(&tcpSocket, &QTcpSocket::disconnected, this, &ServerThread::disconnectionSlot);

    /* A new connection for writing on the DB has to be created. Each thread use its own connection so that
       the atomicity of the transactions is guaranteed
       Using a shared pointer it is guaranteed that the pointer will be destroyed when the thread will die */

    while(tcpSocket.state() == QTcpSocket::ConnectedState){
        if(!tcpSocket.waitForReadyRead(-1))
            break;
        QByteArray buffer = tcpSocket.read(sizeof(int));
        QDataStream ds(buffer);
        int size;
        ds >> size;
        //qDebug() << "Size: " << size << endl;

        /* Timeout of 10 seconds */
        if(!tcpSocket.waitForReadyRead(10000))
            break;

        /* Packet content will be written here */
        QByteArray dataBuffer;        

        dataBuffer = tcpSocket.read(size);
        if(dataBuffer.size() < size){
            qDebug() << "Error on receiving packet" << endl;
            break;
        }        

        /* Conversion from ASCII to QString, according to DB specs */        
        char _buf[256];
        packetCreator(_buf, dataBuffer, size);
        //qDebug() << _buf << endl;

        /* Now I insert the packet into the DB */
        //connection->insertData(_buf);

        connection.insertData(_buf);

        /* TODO Implementare una nuova funzione di hash standard, come MD5 */
        //qDebug() << "Hash string: " << hashFunction(std::string(_buf)).c_str();
        qDebug() << "Packet received" << endl;
    }

    connection.closeConn();

    /* This section is reached if the board has been disconnected from server */
    qDebug() << "The error is: " << tcpSocket.errorString() << endl;
    tcpSocket.disconnectFromHost();
    //tcpSocket.waitForDisconnected();

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



