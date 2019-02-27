#ifndef WORKERTHREADGUI_H
#define WORKERTHREADGUI_H

#include <QThread>
#include "mysqlconn.h"
#include <iostream>
#include <vector>
#include "position.h"

class WorkerThreadGui : public QThread{
    Q_OBJECT
public:
    WorkerThreadGui();
    ~WorkerThreadGui() {
        std::cout << "Worker Gui distruttore" << std::endl;
    }
    void run();
private:
    MySqlConn msconn;

private slots:

signals:
    void paintDevicesSignal(std::vector<Position>);
};
#endif // WORKERTHREADGUI_H
