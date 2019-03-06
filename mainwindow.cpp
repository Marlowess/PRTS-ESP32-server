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
    int x = dw.width() * 0.92;
    int y = dw.height() * 0.92;
    setFixedSize(x,y);


    initializeChart();
    //background-color: rgb(211, 215, 207);
    ui->frame->setStyleSheet(".QFrame{border: 1px solid black; border-radius: 10px; background-color: rgb(211, 215, 207)}");
    ui->label_30->setStyleSheet("font: bold large Times New Roman");
    ui->label_status->setText("DISABLED");
    ui->label_status->setStyleSheet("QLabel { color : red; }");
    ui->lineEdit_5->setPlaceholderText("ESP #1");
    ui->lineEdit_6->setPlaceholderText("ESP #2");
    ui->lineEdit_7->setPlaceholderText("ESP #3");
    ui->lineEdit_8->setPlaceholderText("ESP #4");

    /* Connection to tabWidgets */
    connect(ui->tabWidget, &QTabWidget::tabBarClicked, this, &MainWindow::on_tab_click);

    /*
    server = new Server();
    threadGui = new WorkerThreadGui();
    qRegisterMetaType<QMap<QString, QVector<QString>>>("QMap<QString, QVector<QString>>");
    connect(threadGui, &WorkerThreadGui::paintDevicesSignal, this, &MainWindow::printDevicesSlot);
    threadGui->start();
    */
}

void MainWindow::initializeChart(){
    QChart *chart = new QChart();
    chart->setTheme(QChart::ChartThemeBlueCerulean);

    QScatterSeries *series = new QScatterSeries();
    series->setName("ESP");
    series->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    series->setMarkerSize(10);
    //series->setColor(QColor(51,51,255));
    //series->setPointLabelsColor(QColor(51,51,255));

    QScatterSeries *seriesDevices = new QScatterSeries();
    seriesDevices->setName("Devices");
    seriesDevices->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    seriesDevices->setMarkerSize(10);
    //seriesDevices->setColor(QColor(255,128,0));
    //seriesDevices->setPointLabelsColor(QColor(255,128,0));

    chart->addSeries(series);
    chart->addSeries(seriesDevices);
    chart->createDefaultAxes();
    chart->axisX()->setRange(X_START, X_END);
    chart->axisY()->setRange(Y_START, Y_END);
    ui->graphicsView->setChart(chart);
    ui->graphicsView->setStyleSheet("background-color: rgb(255, 255, 255)}");
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
    if(ui->check_5->isChecked())
        series->append(ui->X_5->value(), ui->Y_5->value());
    else
        series->remove(ui->X_5->value(), ui->Y_5->value());

    // Second checkBox
    if(ui->check_6->isChecked())
        series->append(ui->X_6->value(), ui->Y_6->value());
    else
        series->remove(ui->X_6->value(), ui->Y_6->value());

    // Third checkBox
    if(ui->check_7->isChecked())
        series->append(ui->X_7->value(), ui->Y_7->value());
    else
        series->remove(ui->X_7->value(), ui->Y_7->value());

    // Fourth checkBox
    if(ui->check_8->isChecked())
        series->append(ui->X_8->value(), ui->Y_8->value());
    else
        series->remove(ui->X_8->value(), ui->Y_8->value());


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


void MainWindow::on_check_5_stateChanged(){
    paintBoardsSlot();
    int nBoards = ui->label_boards->text().toInt();
    if(ui->check_5->isChecked()){
        nBoards++;
        threadGui->setBoardsLocation(0, ui->X_5->value(), ui->Y_5->value(), ui->lineEdit_5->text());
        ui->X_5->setEnabled(false);
        ui->Y_5->setEnabled(false);
        ui->lineEdit_5->setEnabled(false);

    }
    else{
        nBoards--;
        threadGui->setBoardsLocation(0, 0, 0, ui->lineEdit_5->text());
        ui->X_5->setEnabled(true);
        ui->Y_5->setEnabled(true);
        ui->lineEdit_5->setEnabled(true);
    }
    ui->label_boards->setText(QString::number(nBoards));
    server->setNumberOfHosts(ui->label_boards->text().toInt()); // setto il numero di board
}

void MainWindow::on_check_6_stateChanged(){
    paintBoardsSlot();
    int nBoards = ui->label_boards->text().toInt();
    if(ui->check_6->isChecked()){
        nBoards++;
        threadGui->setBoardsLocation(1, ui->X_6->value(), ui->Y_6->value(), ui->lineEdit_6->text());
        ui->X_6->setEnabled(false);
        ui->Y_6->setEnabled(false);
        ui->lineEdit_6->setEnabled(false);
    }
    else{
        nBoards--;
        threadGui->setBoardsLocation(1, 0, 0, ui->lineEdit_6->text());
        ui->X_6->setEnabled(true);
        ui->Y_6->setEnabled(true);
        ui->lineEdit_6->setEnabled(true);
    }

    ui->label_boards->setText(QString::number(nBoards));
    server->setNumberOfHosts(ui->label_boards->text().toInt()); // setto il numero di board
}

void MainWindow::on_check_7_stateChanged(){
    paintBoardsSlot();
    int nBoards = ui->label_boards->text().toInt();
    if(ui->check_7->isChecked()){
        nBoards++;
        threadGui->setBoardsLocation(2, ui->X_7->value(), ui->Y_7->value(), ui->lineEdit_7->text());
        ui->X_7->setEnabled(false);
        ui->Y_7->setEnabled(false);
        ui->lineEdit_7->setEnabled(false);
    }
    else{
        nBoards--;
        threadGui->setBoardsLocation(2, 0, 0, ui->lineEdit_7->text());
        ui->X_7->setEnabled(true);
        ui->Y_7->setEnabled(true);
        ui->lineEdit_7->setEnabled(true);
    }
    ui->label_boards->setText(QString::number(nBoards));
    server->setNumberOfHosts(ui->label_boards->text().toInt()); // setto il numero di board
}

void MainWindow::on_check_8_stateChanged(){
    paintBoardsSlot();
    int nBoards = ui->label_boards->text().toInt();
    if(ui->check_8->isChecked()){
        nBoards++;
        threadGui->setBoardsLocation(3, ui->X_8->value(), ui->Y_8->value(), ui->lineEdit_8->text());
        ui->X_8->setEnabled(false);
        ui->Y_8->setEnabled(false);
        ui->lineEdit_8->setEnabled(false);
    }
    else{
        nBoards--;
        threadGui->setBoardsLocation(3, 0, 0, ui->lineEdit_8->text());
        ui->X_8->setEnabled(true);
        ui->Y_8->setEnabled(true);
        ui->lineEdit_8->setEnabled(true);
    }
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
        QStringList list = mac.split('_');
        ui->mac_addresses->setText(list[0].toUpper() + " (" + QString::number(x) + ", " + QString::number(y) + ")");
        ui->label_2->setText("This device has been scanned by " + list[1] + " boards.");
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
    QMap<QString, QVector<QString>>::const_iterator i = map.constBegin();
    while(i != map.constEnd()) {
        //qDebug() << "Chiave:" << i.key() << " Values:" << i.value() << endl;
        QString key = i.key();
        //QString val = i.value()[0];
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

void MainWindow::on_tab_click(int index){
    //qDebug() << "CLICKED TAB " << QString::number(index);
    switch(index){
    case 0:
        break;
    case 1:
        break;
    case 2:
        connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_historical_button_click);
        QChart *chart = new QChart();
        chart->setTheme(QChart::ChartThemeBlueCerulean);
        chart->setTitle("Acme Ltd and BoxWhisk Inc share deviation in 2012");
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->createDefaultAxes();

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignBottom);

        ui->graphicsView_2->setStyleSheet("background-color: rgb(255, 255, 255)}");
        ui->graphicsView_2->setChart(chart);
        return;
    }
}

void MainWindow::on_historical_button_click(){

    //uint msecs = ui->dateTimeEdit->time().msecsTo(ui->dateTimeEdit->time());
    QDateTime start_timestamp = ui->dateTimeEdit->dateTime();
    QDateTime end_timestamp = ui->dateTimeEdit_2->dateTime();


    /* Here I have to put the code to retrieve infos about devices and timestamps */
    hist_thread = new Historical_thread(QString::number(start_timestamp.toTime_t()), QString::number(end_timestamp.toTime_t()));
    qRegisterMetaType<QVector<Historical_device>>("QVector<Historical_device>");
    connect(hist_thread, &Historical_thread::newDataSignal, this, &MainWindow::newHistoricalDataSlot);
    hist_thread->start();
}

void MainWindow::newHistoricalDataSlot(QVector<Historical_device> vec){
    // Here I handle and print data
    for(int i = 0; i < vec.size(); i++)
        qDebug() << "MAC: " << vec[i].getMacAddress() << " START:" << vec[i].getStartTimestamp() << " END: " << vec[i].getEndTimestamp() << " N_TIMES: " << vec[i].getNTimes();

//    QBoxPlotSeries *acmeSeries = new QBoxPlotSeries();
//    acmeSeries->setName("Acme Ltd");

//    QBoxSet *box = new QBoxSet("box1");
//    box->setValue(QBoxSet::LowerExtreme, 20);
//    box->setValue(QBoxSet::UpperExtreme, 25);
//    box->setValue(QBoxSet::Median, 1);
//    box->setValue(QBoxSet::LowerQuartile, 1);
//    box->setValue(QBoxSet::UpperQuartile, 1);

//    QBoxSet *box2 = new QBoxSet("box2");
//    box2->setValue(QBoxSet::LowerExtreme, 17);
//    box2->setValue(QBoxSet::UpperExtreme, 23);
//    box2->setValue(QBoxSet::Median, 1);
//    box2->setValue(QBoxSet::LowerQuartile, 1);
//    box2->setValue(QBoxSet::UpperQuartile, 1);

//    acmeSeries->append(box);
//    acmeSeries->append(box2);

//    QChart *chart = new QChart();
//    chart->setTheme(QChart::ChartThemeBlueCerulean);
//    chart->addSeries(acmeSeries);
//    chart->setTitle("Acme Ltd and BoxWhisk Inc share deviation in 2012");
//    chart->setAnimationOptions(QChart::SeriesAnimations);
//    chart->createDefaultAxes();
//    chart->axes(Qt::Vertical).first()->setMin(15.0);
//    chart->axes(Qt::Horizontal).first()->setMax(34.0);

//    chart->legend()->setVisible(true);
//    chart->legend()->setAlignment(Qt::AlignBottom);

//    ui->graphicsView_2->setStyleSheet("background-color: rgb(255, 255, 255)}");
//    ui->graphicsView_2->setChart(chart);
}


