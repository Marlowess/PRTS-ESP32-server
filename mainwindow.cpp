#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);    

    QDesktopWidget dw;
    int x=dw.width()*0.75;
    int y=dw.height()*0.75;
    setFixedSize(x,y);

    ChartView chartView(this);
    ui->graphicsView->setChart(chartView.chart());
    chartView.chart()->setTitle("Titolo modificato");



    server = new Server();
}

MainWindow::~MainWindow(){
    delete ui;
}

//void MainWindow::on_connectButton_clicked(){
//    server->setPort(ui->lineEdit->text().toInt());
//    server->setConnection();
//    connect(server, SIGNAL(newConnection()), this, SLOT(showConnectionStatus()));
//}

//void MainWindow::showConnectionStatus(){
//    ui->checkConn->setChecked(true);
//}
