#include "workerthreadtab.h"
#include <QDebug>

void WorkerThreadTab::run() {

    unsigned long time = 1000;
    while(true) {
        //mutex->lock();
        while(!*notified) {

            {
                bool res = waitCondition->wait(mutex.data(), time);//qDebug() << res << " " << *notified;
                if(!res == true && !(*notified)) {
                    QList<QPair<QString, double>>* List = doWork();
                    emit signal_data_ready(List);
                    mutex->unlock();
                } else if (res == true && *notified) {
                    qDebug() << "worker thread: notified - pause";
                    qDebug() << "worker thread: pause";
                    break;
                }
            }
            //qDebug() << "new iteration";
        }
        conn.closeConn();
        mutex->unlock();
        bool res2 = false;

        //mutex2->lock();
        while(!*(restarted))  {
            qDebug() << "worker thread: wait restart";
            {

                res2 = waitCondition2->wait(mutex2.data());
                qDebug() << res2 << " " << *restarted;
                if(res2 == true && *(restarted)) {
                    mutex2->unlock();
                    break;
                }
                mutex2->unlock();
                qDebug() << "worker thread: spuria";
            }
        }
        qDebug() << "worker thread: restart";
    }

}

QList<QPair<QString, double>>* WorkerThreadTab::doWork() {
    std::chrono::seconds curr_time, delta;
    QList<QPair<QString, double>>* List = nullptr;    
    delta = getDeltaTime(&curr_time);
    if (delta < std::chrono::seconds(60*5)) {
        conn.openConn("probe_requests_db", "root", "password", "localhost", "Thread-Connection-Temporal");
        List = conn.getNumDevicesByTimestamp(reference_time, curr_time);
        //List = conn.getNumDevicesByTimestamp(std::chrono::seconds(1551862024), std::chrono::seconds(1551862024 + delta.count()));
        List->push_front(QPair<QString, double>("", reference_time.count()));
    }
    else{
        reference_time = curr_time;
    }
    return List;
}

std::chrono::seconds WorkerThreadTab::getDeltaTime(std::chrono::seconds* curr_time) {
    *curr_time = std::chrono::seconds(std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1000));
    std::chrono::seconds delta = std::chrono::seconds(std::chrono::duration_cast<std::chrono::seconds>(*curr_time - reference_time).count());
    return delta;
}
