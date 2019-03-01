#include "mainwindow.h"

#define X_START -6
#define X_END 6
#define Y_START -6
#define Y_END 6

bool online = false; // system ON/OFF

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    QDesktopWidget dw;
    int x = dw.width() * 0.9;
    int y = dw.height() * 0.9;
    setFixedSize(x,y);


    initializeChart();
    ui->frame->setStyleSheet(".QFrame{border: 1px solid black; border-radius: 10px;}");
    ui->label_status->setText("DISABLED");
    ui->label_status->setStyleSheet("QLabel { color : red; }");

    server = new Server();
//    connect(server, &Server::paintDevicesSignal, this, &MainWindow::printDevicesSlot);
    threadGui = new WorkerThreadGui();
    qRegisterMetaType<QMap<QString, QVector<QString>>>("QMap<QString, QVector<QString>>");
    connect(threadGui, &WorkerThreadGui::paintDevicesSignal, this, &MainWindow::printDevicesSlot);
    threadGui->start();

}

void MainWindow::initializeChart(){
    QChart *chart = new QChart();

    QScatterSeries *series = new QScatterSeries();
    series->setName("ESP");
    series->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    series->setMarkerSize(10);
    series->setColor(QColor(51,51,255));
    series->setPointLabelsColor(QColor(51,51,255));

    QScatterSeries *seriesDevices = new QScatterSeries();
    seriesDevices->setName("Devices");
    seriesDevices->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    seriesDevices->setMarkerSize(10);
    seriesDevices->setColor(QColor(255,128,0));
    seriesDevices->setPointLabelsColor(QColor(255,128,0));

    chart->addSeries(series);
    chart->addSeries(seriesDevices);
    chart->createDefaultAxes();
    chart->axisX()->setRange(X_START, X_END);
    chart->axisY()->setRange(Y_START, Y_END);
    ui->graphicsView->setChart(chart);
    ui->label_boards->setText("0");
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::paintBoardsSlot(){
    QScatterSeries *series = new QScatterSeries();
    series->setName("ESP");
    QChart *chart = ui->graphicsView->chart();
    for(QAbstractSeries *q : chart->series())
        if(q->name().compare("ESP") == 0){
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
    chart->axisX()->setRange(X_START, X_END);
    chart->axisY()->setRange(Y_START, Y_END);
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
    int nBoards = ui->label_boards->text().toInt();
    if(ui->check_1->isChecked())
        nBoards++;
    else
        nBoards--;
    ui->label_boards->setText(QString::number(nBoards));
    server->setNumberOfHosts(ui->label_boards->text().toInt()); // setto il numero di board
}

void MainWindow::on_check_2_stateChanged(){
    paintBoardsSlot();
    int nBoards = ui->label_boards->text().toInt();
    if(ui->check_2->isChecked())
        nBoards++;
    else
        nBoards--;
    ui->label_boards->setText(QString::number(nBoards));
    server->setNumberOfHosts(ui->label_boards->text().toInt()); // setto il numero di board
}

void MainWindow::on_check_3_stateChanged(){
    paintBoardsSlot();
    int nBoards = ui->label_boards->text().toInt();
    if(ui->check_3->isChecked())
        nBoards++;
    else
        nBoards--;
    ui->label_boards->setText(QString::number(nBoards));
    server->setNumberOfHosts(ui->label_boards->text().toInt()); // setto il numero di board
}

void MainWindow::on_check_4_stateChanged(){
    paintBoardsSlot();
    int nBoards = ui->label_boards->text().toInt();
    if(ui->check_4->isChecked())
        nBoards++;
    else
        nBoards--;
    ui->label_boards->setText(QString::number(nBoards));
    server->setNumberOfHosts(ui->label_boards->text().toInt()); // setto il numero di board
}


void MainWindow::on_pushButton_clicked(){

    //    server->setPort(ui->lineEdit->text().toInt());
    //    server->setConnection();
    //    connect(server, SIGNAL(newConnection()), this, SLOT(showConnectionStatus()));

    if(online){
        online = false;
        ui->label_status->setText("DISABLED");
        ui->label_status->setStyleSheet("QLabel { color : red; }");
    }
    else{
        online = true;
        ui->label_status->setText("ENABLED");
        ui->label_status->setStyleSheet("QLabel { color : green; }");
    }

    server->setNumberOfHosts(ui->label_boards->text().toInt()); // setto il numero di board
    server->setPort(1026); // setto la porta
    server->setConnection(); // faccio partire la connessione
}

void MainWindow::on_point_clicked(QPointF point){
    //ui->mac_addresses->setText("Questo slot funziona!");
    float x = point.x();
    float y = point.y();
    std::string s = std::to_string(x) + "_" + std::to_string(y);
    QString str(QString::fromStdString(s));
    QVector<QString> vec;
    QString mac = "";
    if(points_map.contains(str)){
        vec = points_map.value(str);
        mac = vec.front();
        ui->mac_addresses->setText(mac);
    }
}

/** This method is invoked each time the server has finished data handling on DB **/
//void MainWindow::printDevicesSlot(std::vector<Position> vector){
//   // qDebug() << "Ready to print devices on chart" << endl;
//    QScatterSeries *series = new QScatterSeries();
//    series->setName("Devices");
//    QChart *chart = ui->graphicsView->chart();
//    for(QAbstractSeries *q : chart->series())
//        if(q->name().compare("Devices") == 0){
//            chart->removeSeries(q);
//            delete q;
//            break;
//        }
//    series->append(x,y);
//    series->append(4,4);
//    series->append(6,6);
//    chart->addSeries(series);
//    chart->createDefaultAxes();
//    chart->axisX()->setRange(0, 10.0);
//    chart->axisY()->setRange(0, 10.0);
//    ui->graphicsView->setChart(chart);
//}

void MainWindow::printDevicesSlot(QMap<QString, QVector<QString>> map){
   // qDebug() << "Ready to print devices on chart" << endl;
    this->points_map = map;
    QScatterSeries *series = new QScatterSeries();
    series->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    series->setName("Devices");
    QChart *chart = ui->graphicsView->chart();
    for(QAbstractSeries *q : chart->series())
        if(q->name().compare("Devices") == 0){
            chart->removeSeries(q);
            delete q;
            break;
        }
//    std::vector<Position>::iterator it;
//    for (it = vec.begin(); it < vec.end(); it++)
//        series->append(it->getX(), it->getY());

    QMap<QString, QVector<QString>>::const_iterator i = map.constBegin();
    while(i != map.constEnd()) {
        //qDebug() << "Chiave:" << i.key() << " Values:" << i.value() << endl;
        QString key = i.key();
        QStringList list = key.split('_');
        qDebug() << "Chiave:" << list[0] << " Values:" << list[1] << endl;
        int iX = list[0].indexOf(",", 0);
        int iY = list[1].indexOf(",", 0);
        double x = list[0].replace(iX, 1, ".").toDouble();
        double y = list[1].replace(iY, 1, ".").toDouble();
        qDebug() << "PosX: " << x << " PosY: " << y << endl;
        series->append(x, y);
        i++;
    }

    connect(series, &QScatterSeries::clicked, this, &MainWindow::on_point_clicked);
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axisX()->setRange(X_START, X_END);
    chart->axisY()->setRange(Y_START, Y_END);
    ui->graphicsView->setChart(chart);
}


