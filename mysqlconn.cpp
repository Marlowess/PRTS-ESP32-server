#include "mysqlconn.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <QFile>
#include <QTextStream>

MySqlConn::MySqlConn(){
    qDebug() << "Create MySqlConn Obj";
}

MySqlConn::~MySqlConn() {
    qDebug() << "Try destroy MySqlConn Obj";
    if ( db_m.isValid() ) {
        db_m.close();
        if (!db_m.connectionName().isNull()) {
            qDebug() << "removing connection" << db_m.connectionName();
            QSqlDatabase::removeDatabase(db_m.connectionName());
         }
    }    
}

bool MySqlConn::openConn(const QString& dbName, const QString& usrName, const QString& password, const QString& hostName) {
    bool ok = false;
    //mutex.lock();
    if (!db_m.isValid()) {
        db_m = QSqlDatabase::addDatabase("QMYSQL", "my-connection");
        db_m.setHostName(hostName);
        db_m.setDatabaseName(dbName);
        db_m.setUserName(usrName);
        db_m.setPassword(password);

        qDebug() << "Try Open connection";
        ok = db_m.open();

        if ( ok ) {
            qDebug() << "Sql connection is open";
        }
        else {
            qDebug() << "Sql connection is not open";
        }

    }
    //mutex.unlock();
    return ok;
}

bool MySqlConn::selectAll() {
    bool res = false;
    //mutex.lock();
    if ( db_m.isValid() && db_m.isOpen() ) {
        qDebug() << "== Start Result selectAll ===";
        QSqlQuery query("SELECT * FROM probe_requests;", db_m);
        int idName = query.record().indexOf("mac_address_device");
        while (query.next()) {
            QString name = query.value(idName).toString();
            if (name.isEmpty()) {
                qDebug() << "=== \"Not Available\"";
            } else {
                qDebug() << "===" << name;
            }
        }
        qDebug() << "== End Result selectAll ===";
        res =  true;
    } else {
        qDebug() << "Query failed";
    }

    //mutex.unlock();
    return res;

}

bool MySqlConn::readFromFile(const QString& fileName) {
    bool res = false;
    if ( fileName.isEmpty() ) return res;
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly |
                      QFile::Text)) {
        qDebug() << "Could not open file " << fileName << " for reading its content";
        return res;
    }

    QTextStream in(&file);
    int i = 0;
    QString line;
    qDebug() << "== Start reading file ==";
    do {
        line = in.readLine();
        if ( line.isNull() ) break;
        //qDebug() << "Line n. " << i << ": "<< line;
        insertProbeRequest(line);
        i++;
    } while( !line.isNull() );
    qDebug() << "== End read file ==";
    file.close();
    return res;
}

bool MySqlConn::insertData(const QString &data){
    insertProbeRequest(data);
}


bool MySqlConn::insertProbeRequest(const QString& probeRequest) {
    bool res = false;
    if (probeRequest.isEmpty()) return false;
    mutex.lock();
    qDebug() << "La stringa che arriva: " << probeRequest << endl;
    if ( db_m.isValid() && db_m.isOpen() ) {
        QStringList list =  probeRequest.split(",");
        //for(int i = 0; i < list.length(); i++) {
            //qDebug() << list.at(i);
        //}
        QSqlQuery query(db_m);
        query.prepare("INSERT INTO probe_requests (mac_address_board, mac_address_device, ssid, timestamp, signal_strength) "
                                  "VALUES (:mac_board, :mac_device, :ssid, :timestamp, :signal_strength);");

        query.bindValue(":mac_board", list.at(0));
        query.bindValue(":mac_device", list.at(1));
        query.bindValue(":ssid", list.at(2));
        query.bindValue(":timestamp", list.at(3));
        query.bindValue(":signal_strength", list.at(4).toInt());

        if (!query.exec()) {
            qDebug() << query.executedQuery();
            qDebug() << "Couldn't exec query for 'probe_request_table' table";
            qDebug() << "Error motivation: " << db_m.lastError();
        } else {
            qDebug() << query.executedQuery();
            qDebug() << "query executed!";
            res = true;
        }
        //res = true;
    } else {
        qDebug() << "Query failed";
    }
    mutex.unlock();
    return res;
}

bool MySqlConn::conn_is_open(){
    return db_m.isOpen();
}
