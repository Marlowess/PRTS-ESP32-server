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
    void printDevicesSlot(std::vector<Position>);


    void on_check_1_stateChanged();

    void on_check_2_stateChanged();

    void on_check_3_stateChanged();

    void on_check_4_stateChanged();

    void on_pushButton_clicked();

    void on_point_clicked(QPointF);

private:
    Ui::MainWindow *ui;
    Server *server;
    WorkerThreadGui *threadGui;
};

#endif // MAINWINDOW_H
