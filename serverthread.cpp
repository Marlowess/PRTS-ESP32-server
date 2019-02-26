#include "serverthread.h"
#include <iostream>

ServerThread::ServerThread(int socketDescriptor, QObject *parent)
    :QThread(parent), socketDescriptor(socketDescriptor){
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
    std::shared_ptr<MySqlConn> connection = std::make_shared<MySqlConn>();

    while(tcpSocket.state() == QTcpSocket::ConnectedState){
        tcpSocket.waitForReadyRead(-1);
        QByteArray buffer = tcpSocket.read(sizeof(int));
        QDataStream ds(buffer);
        int size;
        ds >> size;
        qDebug() << "Size: " << size << endl;

//        if(size == 0){
//            qDebug() << "No data to read" << endl;
//            firstLap = false;
//            break;
//        }

        if(!tcpSocket.waitForReadyRead(4000))
            break;

        /* Packet content will be written here */
        QByteArray dataBuffer;        

        dataBuffer = tcpSocket.read(size);
        if(dataBuffer.size() < size){
            qDebug() << "Error on receiving packet" << endl;
            break;
        }
        qDebug() << dataBuffer << endl;

        /* Conversion from ASCII to QString, according to DB specs */
        QString DataAsString = QTextCodec::codecForMib(1015)->toUnicode(dataBuffer);

        /* If connection to DB is not opened I open it
           I try to open connection until it is established */
        while(!connection->conn_is_open()){
            // Try until the connection is established
            connection->openConn("probe_requests_db", "root", "password", "localhost");
        }

        /* Now I insert the packet into the DB */
        connection->insertData(DataAsString);

        /* REMOVE THIS BEFORE RELEASE! Used for debugging purposes */
        char _buf[256];
        packetCreator(_buf, dataBuffer, size);        

        /* TODO Implementare una nuova funzione di hash standard, come MD5 */
        //qDebug() << "Hash string: " << hashFunction(std::string(_buf)).c_str();
        qDebug() << "Packet received" << endl;
    }

    /* This section is reached if the board has been disconnected from server */
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



