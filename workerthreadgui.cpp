#include "workerthreadgui.h"
#include <QThread>
#include <QDebug>

WorkerThreadGui::WorkerThreadGui(){
}

void WorkerThreadGui::run() {
    bool res = this->msconn.openConn("probe_requests_db", "root", "password", "localhost", "Thread-Connection-Gui");
    //bool res = this->msconn.openConn("probe_requests_db", "root", "password", "localhost");
    if (res == true) {
        while(true) {
            sleep(3);
            emit paintDevicesSignal(this->msconn.selectAll());
            qDebug() << "Go sleeping 3 seconds...";
            // break;
        }
    }

//    sleep(3);
//    std::vector<Position> vect;
//    vect.push_back(Position(1,2));
//    vect.push_back(Position(2,4));
//    vect.push_back(Position(7,9));
//    emit paintDevicesSignal(vect);
//    vect.clear();
//    sleep(3);
//    vect.push_back(Position(3,3));
//    vect.push_back(Position(2,8));
//    vect.push_back(Position(1,4));
//    emit paintDevicesSignal(vect);
}
