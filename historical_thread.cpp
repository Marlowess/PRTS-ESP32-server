#include "historical_thread.h"

Historical_thread::Historical_thread(QString start, QString end, int activity_type){
    this->start_time = start;
    this->end_time = end;
    this->activity_type = activity_type;
}

void Historical_thread::run(){
    /* Here I have to open the connection, get historical data and emit the signal in order to print onto the GUI */
    MySqlConn connection;
    unsigned long now = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1000);
    connection.openConn("probe_requests_db", "root", "password", "localhost", QString::number(now) + qrand());

    switch (activity_type) {
    case 0:{
        QVector<Historical_device> vec_device = connection.getHistoricalData(this->start_time, this->end_time);
        emit newDataSignal(vec_device);
        break;
    }
    case 1:{
        QStringList devices = connection.getDevicesByTime(this->start_time, this->end_time);
        emit devicesListSignal(devices);
        break;
    }
    case 2:{
        QVector<QPointF> points = connection.getPositionsByDevice(this->start_time, this->end_time, this->mac);
        emit devicePositionsSignal(points);
        break;
    }
    case 3:{
        while(true){
            QMap<QString, QVector<QString>> hidden = connection.getHiddenDevices();
            emit hiddenMacsSignal(hidden);
            sleep(3);
        }
    }
    default:
        return;
    }

    connection.closeConn();

}

void Historical_thread::setMacAddress(QString device){
    this->mac = device;
}
