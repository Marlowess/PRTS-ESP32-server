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
#include "historical_device.h"

class MySqlConn{
public:
    MySqlConn();
    bool openConn(const QString& dbName, const QString& usrName, const QString& password, const QString& hostName);
    bool openConn(const QString& dbName, const QString& usrName, const QString& password, const QString& hostName, const QString& connName);
    QMap<QString, QVector<QString>> selectAll(std::shared_ptr<CalculatorDistance>);
    bool readFromFile(const QString& fileName);
    bool insertData(const QString& data);
    bool conn_is_open();
    bool insert_positions_data(QString mac, QString timestamp, float x, float y);
    QVector<Historical_device> getHistoricalData(QString start, QString end);
    bool delete_table(QString time);

    QList<QPair<QString, double>>* getNumDevicesByTimestamp(std::chrono::seconds start, std::chrono::seconds end);
    QList<QPair<QString, double>>* getNumDevicesByTimestamp(void);

    QStringList getDevicesByTime(QString, QString);
    QVector<QPointF> getPositionsByDevice(QString, QString, QString);

    ~MySqlConn();
private:
    bool insertProbeRequest(const QString& probeRequest);
    QMap<QString, QVector<QString>> populate_map(QMap<QString, QVector<QString>>, double, double, QString, int);
    void print_map(QMap<QString, QVector<QString>>);
    QSqlDatabase db_m;
    QMutex mutex;
    unsigned long milliseconds_since_epoch;
};

#endif // MYSQLCONN_H
