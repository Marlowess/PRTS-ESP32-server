#include "workerthreadgui.h"
#include <QThread>
#include <QDebug>

WorkerThreadGui::WorkerThreadGui(){
}

void WorkerThreadGui::run() {
    bool res = this->msconn.openConn("probe_requests_db", "root", "password", "localhost", "Thread-Connection-Gui");    
    calculator = std::make_shared<CalculatorDistance>();
    //CalculatorDistance *c = new CalculatorDistance();
    //bool res = this->msconn.openConn("probe_requests_db", "root", "password", "localhost");
    if (res == true) {
        while(true) {
            sleep(2);
            emit paintDevicesSignal(this->msconn.selectAll(calculator));
        }
    }
}

void WorkerThreadGui::setBoardsLocation(int index, int x, int y, QString mac){
    calculator->setBoardsPosition(index, x, y, mac);
}
