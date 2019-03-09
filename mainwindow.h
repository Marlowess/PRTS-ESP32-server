#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDataStream>
#include <QDesktopWidget>
#include <QFrame>
#include "server.h"
#include <iterator>
#include <exception>
#include <QAbstractSeries>
#include "workerthreadgui.h"
#include "ui_mainwindow.h"
#include <QMap>
#include <QVector>
#include "historical_device.h"
#include "historical_thread.h"

// FRANK ADD LIBs
#include "workerthreadtab.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void initializeChart();
    ~MainWindow();

signals:
    void paintBoardsSignal(std::vector<Position>);    

private slots:
//    void on_connectButton_clicked();
//    void showConnectionStatus();
    void paintBoardsSlot();
    void printDevicesSlot(QMap<QString, QVector<QString>>);

    void on_check_5_stateChanged();
    void on_check_6_stateChanged();
    void on_check_7_stateChanged();
    void on_check_8_stateChanged();
    void on_pushButton_clicked();
    void on_point_clicked(QPointF);
    void on_tab_click(int index);
    void on_historical_button_click();
    void newHistoricalDataSlot(QVector<Historical_device>);
    void on_history_series_click(QPointF);
    void on_movements_devices_click();
    void listDevicesSlot(QStringList);
    void combobox_changed_slot(QString);
    void devicesPositionsSlot(QVector<QPointF> vec);

private:
    Ui::MainWindow *ui;
    Server *server;
    WorkerThreadGui *threadGui;
    Historical_thread *hist_thread;
    QMap<QString, QVector<QString>> points_map;
    QVector<Historical_device> historical_vector;
    void boxPlotFiller(QVector<Historical_device>, QChart *);
    QString historical_timestamp_start;
    QString historical_timestamp_end;
    QString movements_timestamp_start;
    QString movements_timestamp_end;
    //QStringList devicesList;
    QVector<QPointF> devicePositions;

    qreal findMedian(int begin, int end);

    // FRANK ADD ATTRIBUTEs

    QSharedPointer<bool> notfied;

    QSharedPointer<QMutex> mutex;
    QSharedPointer<QWaitCondition> waitCondition;

    QSharedPointer<bool> restart;
    QSharedPointer<QMutex> mutex2;
    QSharedPointer<QWaitCondition> waitCondition2;
    WorkerThreadTab* workerTab_One;

    int old_tab;
    bool tab_2_instantiate = false;
    bool tab_3_instantiate = false;

    // FRANK ADD FUNCTIONs
    void ManageTab1(int);
    void SetMutexsAndCondVars(void);
    void makePlotTab_One(QList<QPair<QString, double>>*);
};

#endif // MAINWINDOW_H
