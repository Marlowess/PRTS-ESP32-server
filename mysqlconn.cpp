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
QMap<QString, QVector<QString>> MySqlConn::selectAll(std::shared_ptr<CalculatorDistance> calc) {
    bool res = false;
    std::vector<Position> vec;
    int index;
    QMap<QString, QVector<QString>> map;
    QString mac_address_device = "", timestamp = "";
    int rssi[4] = {0,0,0,0};
    unsigned long now = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1000);
    unsigned long before = now - 5;
//    unsigned long now = 1551459117;
//    unsigned long before = 1551459117 - 60;
    std::string query_(std::string("select p.mac_address_board, p.mac_address_device, max(p.signal_strength) ")+
                       std::string("from probe_requests p where (p.mac_address_device) in (select p2.mac_address_device from probe_requests p2 ")+
                        std::string("where p2.timestamp >= '") + std::to_string(before) + std::string("' ")+
                         std::string("and p2.timestamp <= '") + std::to_string(now) + std::string("' ")+
                         std::string("group by p2.mac_address_device ")+
                         std::string("order by p2.mac_address_device) ")+
                           std::string("and p.timestamp >= '") + std::to_string(before) + std::string("' ")+
                               std::string("and p.timestamp <= '") + std::to_string(now) + std::string("' ")+
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
        int boardName = query.record().indexOf("mac_address_board");
        int idName = query.record().indexOf("mac_address_device");
        int idRssi = query.record().indexOf("max(p.signal_strength)");
        int i = 0;
        while (query.next()) {
            QString name = query.value(idName).toString();
            QString board = query.value(boardName).toString();
            if(i == 0){
                qDebug() << "   MAC: " << name;
                qDebug() << "   MAC BOARD: " << board;
                mac_address_device = name;
                index = calc->getBoardVec(board);
                qDebug() << "INDICE: " << index;
                rssi[index] = query.value(idRssi).toInt();
                //rssi[i] = query.value(idRssi).toInt();
                i++;
            }
            else{
                if(name == mac_address_device){
                    qDebug() << "+MAC: " << name;
                    index = calc->getBoardVec(board);
                    qDebug() << "INDICE: " << index;
                    rssi[index] = query.value(idRssi).toInt();
                    //rssi[i] = query.value(idRssi).toInt();
                    i++;
                }
                else{
                    if(i > 1){
                        double x, y;
                        //CalculatorDistance calc;
                        //std::vector<int> vec = calc->getBoardVec();

                        int n = calc->getPosition(rssi, &x, &y);
                        //qDebug() << "x: " << x << "  y: " << y << "  Device " << mac_address_device;
                        insert_positions_data(mac_address_device, QString::number(now), x, y);

                        map = populate_map(map, x, y, mac_address_device, n);

                        rssi[0] = 0;
                        rssi[1] = 0;
                        rssi[2] = 0;
                        rssi[3] = 0;
                        //qDebug() << "i = " << QString::fromStdString(std::to_string(i));
                    }
                    mac_address_device = name;
                    index = calc->getBoardVec(board);
                    qDebug() << "INDICE: " << index;
                    rssi[index] = query.value(idRssi).toInt();
                    //rssi[0] = query.value(idRssi).toInt();

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
    //qDebug() << "La stringa che arriva: " << probeRequest << endl;
    if (db_m.isValid() && db_m.isOpen()) {
        mutex.lock();
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
        mutex.unlock();
        //res = true;
    } else {
        qDebug() << "Query failed";
    }    
    return res;
}

bool MySqlConn::conn_is_open(){
    return db_m.isOpen();
}

QMap<QString, QVector<QString>> MySqlConn::populate_map(QMap<QString, QVector<QString>> map, double x, double y, QString mac, int n){
    std::string str = std::to_string(x) + "_" + std::to_string(y);
    QString s(QString::fromStdString(str));
    if(map.contains(s)){
        QVector<QString> vec = map.value(s);
        vec.push_back(mac + "_" + QString::number(n));
        map.remove(s);
        map.insert(s, vec);
    }
    else{
        QVector<QString> vec;
        vec.push_back(mac + "_" + QString::number(n));
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

bool MySqlConn::insert_positions_data(QString mac, QString timestamp, float x, float y){
    bool res = false;
    if (db_m.isValid() && db_m.isOpen()) {
        QSqlQuery query(db_m);
        query.prepare("insert into devices_timestamps_pos(mac_address_device,timestamp,pos_x,pos_y) "
                      "values(:mac_address_device, :timestamp, :x, :y);");

        query.bindValue(":mac_address_device", mac);
        query.bindValue(":timestamp", timestamp);
        query.bindValue(":x", x);
        query.bindValue(":y", y);

        if (!query.exec()) {
//            qDebug() << query.executedQuery();
            qDebug() << "Couldn't exec query for devices_timestamps_pos";
//            qDebug() << "Error motivation: " << db_m.lastError();
        } else {
//            qDebug() << query.executedQuery();
            qDebug() << "Query executed!  Device: " << mac << "  Time: " << timestamp << "(x,y): (" << x << ", " << y << ")";
            res = true;
        }
        mutex.unlock();
        //res = true;
    } else {
        qDebug() << "Query failed";
    }
    return res;
}

QVector<Historical_device> MySqlConn::getHistoricalData(QString start, QString end){
    QVector<Historical_device> vec;
    QString query_string("select distinct mac_address_device, min(timestamp), max(timestamp), count(*) as count "
                                   "from devices_timestamps_pos "
                                   "where timestamp > " + start + " and timestamp < " + end + " "
                                   "group by mac_address_device "
                                   "order by count desc;");
    if ( db_m.isValid() && db_m.isOpen() ) {
        QSqlQuery query(query_string, db_m);
        if (!query.exec()){
              qDebug() << query.lastError();
              return vec;
        }
        int idName = query.record().indexOf("mac_address_device");
        int idStart = query.record().indexOf("min(timestamp)");
        int idEnd = query.record().indexOf("max(timestamp)");
        int n = query.record().indexOf("count");
        int i = 0;
        while (query.next()) {
            if(i > 4) break;
            Historical_device device(query.value(idStart).toString(), query.value(idEnd).toString(), query.value(idName).toString(), query.value(n).toInt());
            vec.push_back(device);
            i++;
        }
    }
    return vec;
}

QList<QPair<QString, double>>* MySqlConn::getNumDevicesByTimestamp(std::chrono::seconds start, std::chrono::seconds end) {
    QList<QPair<QString, double>>* List = nullptr;
    if ( db_m.isValid() && db_m.isOpen() ) {

        //QSqlQuery query("SELECT timestamp, count(distinct mac_address_device) as total FROM probe_requests group by timestamp order by timestamp desc LIMIT 10 ;", db_m);
        //QSqlQuery query("select timestamp, count(*) as total from devices_timestamps_pos group by timestamp order by timestamp;", db_m);
        qDebug() << QString::fromStdString(std::to_string(start.count())) << " " << QString::fromStdString(std::to_string(end.count()));
        QSqlQuery query(this->db_m);
        query.prepare("select timestamp, count(*) as total "
                                                     "from devices_timestamps_pos "
                                                     "where timestamp > :start and timestamp < :end "
                                                     "group by timestamp order by timestamp;");
            query.bindValue(":start", QString::fromStdString(std::to_string(start.count())));
            query.bindValue(":end", QString::fromStdString(std::to_string(end.count())));
            query.exec();
        //QSqlQuery query("select timestamp, count(*) as total from devices_timestamps_pos group by timestamp order by timestamp;", db_m);

        int id_pos_x = query.record().indexOf("timestamp");
        int id_pos_y = query.record().indexOf("total");
        List = new QList<QPair<QString, double>>();
        qDebug() << "== Start Result selectAll ===";        
        while (query.next()) {
            QString timestamp = query.value(id_pos_x).toString();
            int total = query.value(id_pos_y).toInt();
            List->append(QPair<QString, double>(timestamp, total));
            qDebug() << timestamp << " " << total;
            //query.finish();           
        }
        qDebug() << "== End Result selectAll ===";
    } else {
        qDebug() << "Query failed";
    }
    return List;
}

/** This method returns all devices scanned among the defined time interval **/
QStringList MySqlConn::getDevicesByTime(QString start, QString end){
    QStringList devices;
    devices.append("-");
    QString query_string("select distinct mac_address_device "
                                   "from devices_timestamps_pos "
                                   "where timestamp > " + start + " and timestamp < " + end + ";");

    if ( db_m.isValid() && db_m.isOpen() ) {
        QSqlQuery query(query_string, db_m);
        if (!query.exec()){
              qDebug() << query.lastError();
              return devices;
        }
        int idName = query.record().indexOf("mac_address_device");
        while (query.next()) {
            devices.append(query.value(idName).toString().toUpper());
        }
    }
    return devices;
}

QVector<QPointF> MySqlConn::getPositionsByDevice(QString start, QString end, QString mac){
    QVector<QPointF> positions;
    QString query_string("select distinct pos_x, pos_y "
                           "from devices_timestamps_pos "
                           "where timestamp > " + start + " and timestamp < " + end + " "
                           "and mac_address_device = '" + mac + "';");

    if ( db_m.isValid() && db_m.isOpen() ) {
        QSqlQuery query(query_string, db_m);
        if (!query.exec()){
              qDebug() << query.lastError();
              return positions;
        }
        int idX = query.record().indexOf("pos_x");
        int idY = query.record().indexOf("pos_y");
        while (query.next()) {
            positions.append(QPointF(query.value(idX).toFloat(), query.value(idY).toFloat()));
        }
    }
    return positions;
}

bool MySqlConn::closeConn(){
    db_m.close();
}







