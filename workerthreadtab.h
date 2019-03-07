#ifndef WORKERTHREADTAB_H
#define WORKERTHREADTAB_H

#include <QThread>
#include <QWaitCondition>
#include <QMutex>

#include "mysqlconn.h"

class WorkerThreadTab : public QThread
{
    Q_OBJECT
public:
    WorkerThreadTab(QSharedPointer<bool> _notified, QSharedPointer<QMutex> _mutex, QSharedPointer<QWaitCondition> _waitCondition,
                    QSharedPointer<bool> _restarted, QSharedPointer<QMutex> _mutex2, QSharedPointer<QWaitCondition> _waitCondition2
                    ) {
        qDebug() << "worker thread: new";
        this->mutex = _mutex;
        this->mutex2 = _mutex2;
        this->notified = _notified;

        this->waitCondition = _waitCondition;
        this->waitCondition2 = _waitCondition2;
        this->restarted = _restarted;

        reference_time = std::chrono::seconds(std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1000));
        qDebug() << QString::fromStdString(std::to_string(reference_time.count()));
    }
    void run() override;
    QList<QPair<QString, double>>* doWork();

signals:
    void signal_data_ready(QList<QPair<QString, double>>*);

private:
    MySqlConn conn;

    QSharedPointer<bool> notified;
    QSharedPointer<QMutex> mutex;
    QSharedPointer<QWaitCondition> waitCondition;

    QSharedPointer<bool> restarted;
    QSharedPointer<QMutex> mutex2;
    QSharedPointer<QWaitCondition> waitCondition2;

    std::chrono::seconds reference_time;

    std::chrono::seconds getDeltaTime(std::chrono::seconds*);
};

#endif // WORKERTHREADTAB_H
