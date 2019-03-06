#include <QString>

#ifndef HISTORICAL_DEVICE_H
#define HISTORICAL_DEVICE_H

/* This class represents the information about a point */
class Historical_device{
public:
    Historical_device(){}
    Historical_device(QString, QString, QString, int);
    QString getStartTimestamp();
    QString getEndTimestamp();
    QString getMacAddress();
    int getNTimes();

private:
    QString start_timestamp;
    QString end_timestamp;
    QString mac_address;
    int n_times;
};

#endif // HISTORICAL_DEVICE_H
