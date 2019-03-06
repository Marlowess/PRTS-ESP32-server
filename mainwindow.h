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

private:
    Ui::MainWindow *ui;
    Server *server;
    WorkerThreadGui *threadGui;
    Historical_thread *hist_thread;
    QMap<QString, QVector<QString>> points_map;    
};

#endif // MAINWINDOW_H
