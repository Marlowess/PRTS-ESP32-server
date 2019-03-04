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
    void setBoardsLocation(int index, int x, int y);
private:
    MySqlConn msconn;
    std::shared_ptr<CalculatorDistance> calculator;

private slots:    
signals:
    void paintDevicesSignal(QMap<QString, QVector<QString>>);
};
#endif // WORKERTHREADGUI_H
