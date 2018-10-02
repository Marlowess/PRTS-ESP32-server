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
    void paintBoardsSignal();

private slots:
//    void on_connectButton_clicked();
//    void showConnectionStatus();
    void paintBoardsSlot();
    void printDevicesSlot();


    void on_check_1_stateChanged();

    void on_check_2_stateChanged();

    void on_check_3_stateChanged();

    void on_check_4_stateChanged();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    Server *server;
};

#endif // MAINWINDOW_H
