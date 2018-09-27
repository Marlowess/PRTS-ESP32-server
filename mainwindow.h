#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDataStream>
#include <QDesktopWidget>
#include <QFrame>
#include "server.h"
#include "chartview.h"

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
//    void on_connectButton_clicked();
//    void showConnectionStatus();

private:
    Ui::MainWindow *ui;
    Server *server;
};

#endif // MAINWINDOW_H
