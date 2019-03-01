#ifndef MYSQLCONN_H
#define MYSQLCONN_H

#include <QSqlDatabase>
#include <QString>
#include <QMutex>
#include <vector>
#include "position.h"
#include "calculatordistance.h"
#include <ctime>
#include <memory>
#include <QPoint>

class MySqlConn{
public:
    MySqlConn();
    bool openConn(const QString& dbName, const QString& usrName, const QString& password, const QString& hostName);
    bool openConn(const QString& dbName, const QString& usrName, const QString& password, const QString& hostName, const QString& connName);
    QMap<QString, QVector<QString>> selectAll();
    bool readFromFile(const QString& fileName);
    bool insertData(const QString& data);
    bool conn_is_open();
    ~MySqlConn();
private:
    bool insertProbeRequest(const QString& probeRequest);
    void populate_map(std::shared_ptr<QMap<QString, QVector<QString>>>, double, double, QString);
    void print_map(std::shared_ptr<QMap<QString, QVector<QString>>>);
    QSqlDatabase db_m;
    QMutex mutex;
    unsigned long milliseconds_since_epoch;
};

#endif // MYSQLCONN_H
