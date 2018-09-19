#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    showMaximized();
    server = new Server();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_connectButton_clicked(){
    server->setPort(ui->lineEdit->text().toInt());
    server->setConnection();
    connect(server, SIGNAL(newConnection()), this, SLOT(showConnectionStatus()));
}

void MainWindow::showConnectionStatus(){
    ui->checkConn->setChecked(true);
}

void MainWindow::on_timeButton_clicked(){
    ui->editTime->setPlainText(QString::number(server->getSystemTime()));
}
