#ifndef MYSQLCONN_H
#define MYSQLCONN_H

#include <QSqlDatabase>
#include <QString>
#include <QMutex>
#include <vector>
#include "position.h"
#include "calculatordistance.h"

class MySqlConn{
public:
    MySqlConn();
    bool openConn(const QString& dbName, const QString& usrName, const QString& password, const QString& hostName);
    std::vector<Position> selectAll();
    bool readFromFile(const QString& fileName);
    bool insertData(const QString& data);
    bool conn_is_open();
    ~MySqlConn();
private:
    bool insertProbeRequest(const QString& probeRequest);
    QSqlDatabase db_m;
    QMutex mutex;
};

#endif // MYSQLCONN_H
