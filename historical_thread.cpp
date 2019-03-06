#include "historical_thread.h"

Historical_thread::Historical_thread(QString start, QString end){
    this->start_time = start;
    this->end_time = end;
}

void Historical_thread::run(){
    /* Here I have to open the connection, get historical data and emit the signal in order to print onto the GUI */
    MySqlConn connection;
    unsigned long now = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1000);
    connection.openConn("probe_requests_db", "root", "password", "localhost", QString::number(now) + qrand());

    QVector<Historical_device> vec_device = connection.getHistoricalData(this->start_time, this->end_time);
    emit newDataSignal(vec_device);
}
