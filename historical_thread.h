#include <QThread>
#include "mysqlconn.h"
#include "historical_device.h"

#ifndef HISTORICAL_THREAD_H
#define HISTORICAL_THREAD_H


class Historical_thread  : public QThread{
    Q_OBJECT
public:
    Historical_thread(QString, QString);
    void run() override;
private:
    QString start_time;
    QString end_time;
signals:
    void newDataSignal(QVector<Historical_device>);
};

#endif // HISTORICAL_THREAD_H
