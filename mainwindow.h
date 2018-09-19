#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_connectButton_clicked();
    void showConnectionStatus();

    void on_timeButton_clicked();

private:
    Ui::MainWindow *ui;
    Server *server;
};

#endif // MAINWINDOW_H
