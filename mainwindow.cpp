#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    QDesktopWidget dw;
    int x=dw.width()*0.9;
    int y=dw.height()*0.9;
    setFixedSize(x,y);

    initializeChart();
    ui->frame->setStyleSheet(".QFrame{border: 1px solid black; border-radius: 10px;}");

    //server = new Server();
}

void MainWindow::initializeChart(){
    QChart *chart = new QChart();

    QScatterSeries *series = new QScatterSeries();
    series->setName("Boards");
    series->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    series->setMarkerSize(10);
    series->setColor(QColor(51,51,255));
    series->setPointLabelsColor(QColor(51,51,255));

    QScatterSeries *seriesDevices = new QScatterSeries();
    seriesDevices->setName("Devices");
    seriesDevices->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    seriesDevices->setMarkerSize(10);
    seriesDevices->setColor(QColor(255,128,0));
    seriesDevices->setPointLabelsColor(QColor(255,128,0));

    chart->addSeries(series);
    chart->addSeries(seriesDevices);
    chart->createDefaultAxes();
    chart->axisX()->setRange(0, 10.0);
    chart->axisY()->setRange(0, 10.0);
    ui->graphicsView->setChart(chart);

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::paintBoardsSlot(){
    QScatterSeries *series = new QScatterSeries();
    series->setName("Boards");
    QChart *chart = ui->graphicsView->chart();
    for(QAbstractSeries *q : chart->series())
        if(q->name().compare("Boards") == 0){
            chart->removeSeries(q);
            delete q;
            break;
        }

    // First checkBox
    if(ui->check_1->isChecked())
        series->append(ui->X_1->value(), ui->Y_1->value());
    else
        series->remove(ui->X_1->value(), ui->Y_1->value());

    // Second checkBox
    if(ui->check_2->isChecked())
        series->append(ui->X_2->value(), ui->Y_2->value());
    else
        series->remove(ui->X_2->value(), ui->Y_2->value());

    // Third checkBox
    if(ui->check_3->isChecked())
        series->append(ui->X_3->value(), ui->Y_3->value());
    else
        series->remove(ui->X_3->value(), ui->Y_3->value());

    // Fourth checkBox
    if(ui->check_4->isChecked())
        series->append(ui->X_4->value(), ui->Y_4->value());
    else
        series->remove(ui->X_4->value(), ui->Y_4->value());


    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axisX()->setRange(0, 10.0);
    chart->axisY()->setRange(0, 10.0);
    ui->graphicsView->setChart(chart);
}

//void MainWindow::on_connectButton_clicked(){
//    server->setPort(ui->lineEdit->text().toInt());
//    server->setConnection();
//    connect(server, SIGNAL(newConnection()), this, SLOT(showConnectionStatus()));
//}

//void MainWindow::showConnectionStatus(){
//    ui->checkConn->setChecked(true);
//}


void MainWindow::on_check_1_stateChanged(){
    paintBoardsSlot();
}

void MainWindow::on_check_2_stateChanged(){
    paintBoardsSlot();
}

void MainWindow::on_check_3_stateChanged(){
    paintBoardsSlot();
}

void MainWindow::on_check_4_stateChanged(){
    paintBoardsSlot();
}
