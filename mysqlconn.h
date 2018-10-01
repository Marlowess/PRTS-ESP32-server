#ifndef MYSQLCONN_H
#define MYSQLCONN_H

#include <QSqlDatabase>
#include <QString>
#include <QMutex>

class MySqlConn
{
public:
    MySqlConn();
    bool openConn(const QString& dbName, const QString& usrName, const QString& password, const QString& hostName);
    bool selectAll();
    bool readFromFile(const QString& fileName);
    ~MySqlConn();
private:
    bool insertProbeRequest(const QString& probeRequest);

    QSqlDatabase db_m;
    //QMutex mutex;
};

#endif // MYSQLCONN_H
