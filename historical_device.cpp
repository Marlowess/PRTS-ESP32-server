#include "historical_device.h"

Historical_device::Historical_device(QString start, QString end, QString mac, int n_times){
    start_timestamp = start;
    end_timestamp = end;
    mac_address = mac;
    this->n_times = n_times;
}

QString Historical_device::getStartTimestamp(){
    return start_timestamp;
}

QString Historical_device::getEndTimestamp(){
    return end_timestamp;
}

QString Historical_device::getMacAddress(){
    return mac_address;
}

int Historical_device::getNTimes(){
    return n_times;
}
