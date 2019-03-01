#include "mysqlconn.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include <QFile>
#include <QTextStream>
#include <chrono>

MySqlConn::MySqlConn(){
    qDebug() << "Create MySqlConn Obj";
//    time = std::time(nullptr);
//        qDebug() << std::asctime(std::localtime(&time))
//                  << time << " seconds since the Epoch\n";

//    milliseconds_since_epoch = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1000);
//    qDebug() << milliseconds_since_epoch << endl;
//    unsigned long t = milliseconds_since_epoch - 5000;
//    qDebug() << t << endl;

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

bool MySqlConn::openConn(const QString& dbName, const QString& usrName, const QString& password, const QString& hostName, const QString& connName) {
    bool ok = false;
    //mutex.lock();
    if (!db_m.isValid()) {
        db_m = QSqlDatabase::addDatabase("QMYSQL", connName);
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

//bool MySqlConn::selectAll() {
//    bool res = false;
//    //mutex.lock();
//    if ( db_m.isValid() && db_m.isOpen() ) {
//        qDebug() << "== Start Result selectAll ===";
//        QSqlQuery query("SELECT * FROM probe_requests;", db_m);
//        int idName = query.record().indexOf("mac_address_device");
//        while (query.next()) {
//            QString name = query.value(idName).toString();
//            if (name.isEmpty()) {
//                qDebug() << "=== \"Not Available\"";
//            } else {
//                qDebug() << "===" << name;
//            }
//        }
//        qDebug() << "== End Result selectAll ===";
//        res =  true;
//    } else {
//        qDebug() << "Query failed";
//    }

//    //mutex.unlock();
//    return res;
//}
QMap<QString, QVector<QString>> MySqlConn::selectAll() {
    bool res = false;
    std::vector<Position> vec;
    QMap<QString, QVector<QString>> map;
    QString mac_address_device = "", timestamp = "";
    int rssi[4] = {0,0,0,0};
    unsigned long now = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1000);
    unsigned long before = now - 60;
//    unsigned long now = 1551459117;
//    unsigned long before = 1551459117 - 60;
    std::string query_(std::string("select p.mac_address_board, p.mac_address_device, min(p.signal_strength) ")+
                       std::string("from probe_requests p where (p.mac_address_device) in (select p2.mac_address_device from probe_requests p2 ")+
                        std::string("where p2.timestamp > '") + std::to_string(before) + std::string("' ")+
                         std::string("and p2.timestamp < '") + std::to_string(now) + std::string("' ")+
                         std::string("group by p2.mac_address_device ")+
                         std::string("order by p2.mac_address_device) ")+
                           std::string("and p.timestamp > '") + std::to_string(before) + std::string("' ")+
                               std::string("and p.timestamp < '") + std::to_string(now) + std::string("' ")+
                       std::string("group by mac_address_board, mac_address_device ")+
                       std::string("order by p.mac_address_device;"));
    QString query_string(QString::fromStdString(query_));
    //mutex.lock();
    if ( db_m.isValid() && db_m.isOpen() ) {
        qDebug() << "== Start Result selectAll ===";
        QSqlQuery query(query_string, db_m);        
        if (!query.exec()){
              qDebug() << query.lastError();
              return map;
        }
        int idName = query.record().indexOf("mac_address_device");
        int idRssi = query.record().indexOf("min(p.signal_strength)");
        int i = 0;
        while (query.next()) {
            QString name = query.value(idName).toString();
            if(i == 0){
                qDebug() << "   MAC: " << name;
                mac_address_device = name;
                rssi[i] = query.value(idRssi).toInt();                
                i++;
            }
            else{                
                if(name == mac_address_device){
                    qDebug() << "+MAC: " << name;
                    rssi[i] = query.value(idRssi).toInt();
                    i++;
                }
                else{
                    if(i > 1){
                        double x, y;
                        CalculatorDistance calc;
                        calc.getPosition(rssi[0], rssi[1], rssi[2], rssi[3], &x, &y);
                        //qDebug() << "x: " << x << "  y: " << y << "  Device " << mac_address_device;

                        map = populate_map(map, x, y, mac_address_device);

                        rssi[0] = 0;
                        rssi[1] = 0;
                        rssi[2] = 0;
                        rssi[3] = 0;
                        qDebug() << "i = " << QString::fromStdString(std::to_string(i));
                    }
                    mac_address_device = name;
                    rssi[0] = query.value(idRssi).toInt();

                    i = 1;
                    qDebug() << endl;
                    qDebug() << "   MAC: " << name;
                }
            }
        }
        qDebug() << "== End Result selectAll ===";
        qDebug() << "Now: " << now << "  Before: " << before;
        print_map(map);
        res =  true;
    } else {
        qDebug() << "Query failed";
    }

    //mutex.unlock();
    return map;
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
    return insertProbeRequest(data);
}


bool MySqlConn::insertProbeRequest(const QString& probeRequest) {
    bool res = false;
    if (probeRequest.isEmpty()) return false;
    mutex.lock();
    //qDebug() << "La stringa che arriva: " << probeRequest << endl;
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
//            qDebug() << query.executedQuery();
//            qDebug() << "Couldn't exec query for 'probe_request_table' table";
//            qDebug() << "Error motivation: " << db_m.lastError();
        } else {
//            qDebug() << query.executedQuery();
            qDebug() << "Query executed! Board: " << list.at(0) << " Device: " << list.at(1) << "  Time: " << list.at(3);
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

QMap<QString, QVector<QString>> MySqlConn::populate_map(QMap<QString, QVector<QString>> map, double x, double y, QString mac){
    std::string str = std::to_string(x) + "_" + std::to_string(y);
    QString s(QString::fromStdString(str));
    if(map.contains(s)){
        QVector<QString> vec = map.value(s);
        vec.push_back(mac);
        map.remove(s);
        map.insert(s, vec);
    }
    else{
        QVector<QString> vec;
        vec.push_back(mac);
        map.insert(s, vec);
    }
    return map;
}

void MySqlConn::print_map(QMap<QString, QVector<QString>> map){
    qDebug() << "Map size: " << map.size() << endl;
    QMap<QString, QVector<QString>>::const_iterator i = map.constBegin();
    while(i != map.constEnd()) {
        qDebug() << "Chiave:" << i.key() << " Values:" << i.value() << endl;
        i++;
    }
}
