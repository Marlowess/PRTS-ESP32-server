#include <QThread>
#include "mysqlconn.h"
#include "historical_device.h"

#ifndef HISTORICAL_THREAD_H
#define HISTORICAL_THREAD_H


class Historical_thread  : public QThread{
    Q_OBJECT
public:
    Historical_thread(QString, QString, int);
    void run() override;
    void setMacAddress(QString);
private:
    QString start_time;
    QString end_time;
    QString mac;

/*
    0 means historical data
    1 means get list of devices
    2 means get all positions of the choosen device in the time interval
*/
    int activity_type;
signals:
    void newDataSignal(QVector<Historical_device>);
    void devicesListSignal(QStringList);
    void devicePositionsSignal(QVector<QPointF>);
};

#endif // HISTORICAL_THREAD_H
