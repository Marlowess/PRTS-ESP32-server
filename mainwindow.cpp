#include "mainwindow.h"
#include "qcustomplot.h"

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

    /* Connection between historical button and tab */
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_historical_button_click);

    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::on_movements_devices_click);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &MainWindow::combobox_changed_slot);

    server = new Server();
    threadGui = new WorkerThreadGui();
    qRegisterMetaType<QMap<QString, QVector<QString>>>("QMap<QString, QVector<QString>>");
    connect(threadGui, &WorkerThreadGui::paintDevicesSignal, this, &MainWindow::printDevicesSlot);
    threadGui->start();

    this->SetMutexsAndCondVars();
}

void MainWindow::initializeChart(){
    QChart *chart = new QChart();
    chart->setTheme(QChart::ChartThemeBlueCerulean);

    QScatterSeries *series = new QScatterSeries();
    series->setName("ESP");
    series->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    series->setMarkerSize(10);

    QScatterSeries *seriesDevices = new QScatterSeries();
    seriesDevices->setName("Devices");
    seriesDevices->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    seriesDevices->setMarkerSize(10);

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
    if(workerTab_One != nullptr) {
        delete  workerTab_One;
    }
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

void MainWindow::on_check_5_stateChanged(){   
    int nBoards = ui->label_boards->text().toInt();
    if(ui->check_5->isChecked()){
        if(ui->lineEdit_5->text().isEmpty()
                || (!ui->X_5->value() && !ui->Y_5->value())){
            return;
        }
        nBoards++;
        threadGui->setBoardsLocation(0, ui->X_5->value(), ui->Y_5->value(), ui->lineEdit_5->text());
        ui->X_5->setEnabled(false);
        ui->Y_5->setEnabled(false);
        ui->lineEdit_5->setEnabled(false);

    }
    else{
        if(ui->lineEdit_5->text().isEmpty()
                || (!ui->X_5->value() && !ui->Y_5->value())){
            return;
        }
        nBoards--;
        threadGui->setBoardsLocation(0, 0, 0, ui->lineEdit_5->text());
        ui->X_5->setEnabled(true);
        ui->Y_5->setEnabled(true);
        ui->lineEdit_5->setEnabled(true);
    }
    paintBoardsSlot();
    ui->label_boards->setText(QString::number(nBoards));
    server->setNumberOfHosts(ui->label_boards->text().toInt()); // setto il numero di board
}

void MainWindow::on_check_6_stateChanged(){    
    int nBoards = ui->label_boards->text().toInt();
    if(ui->check_6->isChecked()){
        if(ui->lineEdit_6->text().isEmpty()
                || (!ui->X_6->value() && !ui->Y_6->value())){
            return;
        }
        nBoards++;
        threadGui->setBoardsLocation(1, ui->X_6->value(), ui->Y_6->value(), ui->lineEdit_6->text());
        ui->X_6->setEnabled(false);
        ui->Y_6->setEnabled(false);
        ui->lineEdit_6->setEnabled(false);
    }
    else{
        if(ui->lineEdit_6->text().isEmpty()
                || (!ui->X_6->value() && !ui->Y_6->value())){
            return;
        }
        nBoards--;
        threadGui->setBoardsLocation(1, 0, 0, ui->lineEdit_6->text());
        ui->X_6->setEnabled(true);
        ui->Y_6->setEnabled(true);
        ui->lineEdit_6->setEnabled(true);
    }
    paintBoardsSlot();
    ui->label_boards->setText(QString::number(nBoards));
    server->setNumberOfHosts(ui->label_boards->text().toInt()); // setto il numero di board
}

void MainWindow::on_check_7_stateChanged(){    
    int nBoards = ui->label_boards->text().toInt();
    if(ui->check_7->isChecked()){
        if(ui->lineEdit_7->text().isEmpty()
                || (!ui->X_7->value() && !ui->Y_7->value())){
            return;
        }
        nBoards++;
        threadGui->setBoardsLocation(2, ui->X_7->value(), ui->Y_7->value(), ui->lineEdit_7->text());
        ui->X_7->setEnabled(false);
        ui->Y_7->setEnabled(false);
        ui->lineEdit_7->setEnabled(false);
    }
    else{
        if(ui->lineEdit_7->text().isEmpty()
                || (!ui->X_7->value() && !ui->Y_7->value())){
            return;
        }
        nBoards--;
        threadGui->setBoardsLocation(2, 0, 0, ui->lineEdit_7->text());
        ui->X_7->setEnabled(true);
        ui->Y_7->setEnabled(true);
        ui->lineEdit_7->setEnabled(true);
    }
    paintBoardsSlot();
    ui->label_boards->setText(QString::number(nBoards));
    server->setNumberOfHosts(ui->label_boards->text().toInt()); // setto il numero di board
}

void MainWindow::on_check_8_stateChanged(){    
    int nBoards = ui->label_boards->text().toInt();
    if(ui->check_8->isChecked()){
        if(ui->lineEdit_8->text().isEmpty()
                || (!ui->X_8->value() && !ui->Y_8->value())){
            return;
        }
        nBoards++;
        threadGui->setBoardsLocation(3, ui->X_8->value(), ui->Y_8->value(), ui->lineEdit_8->text());
        ui->X_8->setEnabled(false);
        ui->Y_8->setEnabled(false);
        ui->lineEdit_8->setEnabled(false);
    }
    else{
        if(ui->lineEdit_8->text().isEmpty()
                || (!ui->X_8->value() && !ui->Y_8->value())){
            return;
        }
        nBoards--;
        threadGui->setBoardsLocation(3, 0, 0, ui->lineEdit_8->text());
        ui->X_8->setEnabled(true);
        ui->Y_8->setEnabled(true);
        ui->lineEdit_8->setEnabled(true);
    }
    paintBoardsSlot();
    ui->label_boards->setText(QString::number(nBoards));
    server->setNumberOfHosts(ui->label_boards->text().toInt()); // setto il numero di board
}


void MainWindow::on_pushButton_clicked(){
    ui->pushButton->setEnabled(false);
    ui->label_status->setText("ENABLED");
    ui->label_status->setStyleSheet("QLabel { color : green; }");

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
        this->ManageTab1(index);
        //break;
        this->old_tab = index;
        return;
    case 2:
        //if(old_tab == 2) break;
        if(!tab_2_instantiate){
            tab_2_instantiate = true;
            QChart *chart = new QChart();
            chart->setTheme(QChart::ChartThemeBlueCerulean);
            chart->setTitle("Long time statistics");
            chart->setAnimationOptions(QChart::SeriesAnimations);
            chart->createDefaultAxes();

            chart->legend()->setVisible(true);
            chart->legend()->setAlignment(Qt::AlignBottom);

            ui->graphicsView_2->setStyleSheet("background-color: rgb(255, 255, 255)}");
            ui->graphicsView_2->setChart(chart);

            ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
            ui->dateTimeEdit_2->setDateTime(QDateTime::currentDateTime());

            //connect()
        }
        break;
    case 3:
        if(!tab_3_instantiate){
            tab_3_instantiate = true;
            QChart *chart = new QChart();
            chart->setTheme(QChart::ChartThemeBlueCerulean);
            chart->setTitle("Movements of devices");
            chart->setAnimationOptions(QChart::SeriesAnimations);
            chart->createDefaultAxes();

            chart->legend()->setVisible(true);
            chart->legend()->setAlignment(Qt::AlignBottom);

            ui->graphicsView_3->setStyleSheet("background-color: rgb(255, 255, 255)}");
            ui->graphicsView_3->setChart(chart);

            ui->dateTimeEdit_3->setDateTime(QDateTime::currentDateTime());
            ui->dateTimeEdit_4->setDateTime(QDateTime::currentDateTime());
        }

    }
    this->old_tab = index;
    this->ManageTab1(index);
    return;
}

void MainWindow::on_historical_button_click(){

    //uint msecs = ui->dateTimeEdit->time().msecsTo(ui->dateTimeEdit->time());
    QDateTime start_timestamp = ui->dateTimeEdit->dateTime();
    QDateTime end_timestamp = ui->dateTimeEdit_2->dateTime();

    this->historical_timestamp_start = QString::number(start_timestamp.toTime_t());
    this->historical_timestamp_end = QString::number(end_timestamp.toTime_t());

    /* Here I have to put the code to retrieve infos about devices and timestamps */
    hist_thread = new Historical_thread(QString::number(start_timestamp.toTime_t()), QString::number(end_timestamp.toTime_t()), 0);
    qRegisterMetaType<QVector<Historical_device>>("QVector<Historical_device>");
    connect(hist_thread, &Historical_thread::newDataSignal, this, &MainWindow::newHistoricalDataSlot);
    hist_thread->start();
}

void MainWindow::newHistoricalDataSlot(QVector<Historical_device> vec){

    if(vec.size() == 0) {
        ui->label_10->setVisible(true);
        ui->label_10->setText("NO DATA");
        return;
    }

    ui->label_10->setVisible(false);
    historical_vector = vec;

    QChart *chart = new QChart();
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    chart->setTitle("Historical Data");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    boxPlotFiller(vec, chart);

    chart->createDefaultAxes();
    chart->axisX()->setRange(0, vec.size()+1);
    chart->axisX()->setLabelsVisible(false);
    chart->axisY()->setRange(historical_timestamp_start, historical_timestamp_end);
    chart->axisY()->setLabelsVisible(false);
    chart->axisY()->setTitleText("Time interval");

    ui->graphicsView_2->setStyleSheet("background-color: rgb(255, 255, 255)}");
    ui->graphicsView_2->setChart(chart);
}

void MainWindow::boxPlotFiller(QVector<Historical_device> vec, QChart *chart){
    for(int i = 0; i < vec.size(); i++){
        QString mac = vec[i].getMacAddress();
        int startTime = vec[i].getStartTimestamp().toInt();
        int endTime = vec[i].getEndTimestamp().toInt();
        int nTimes = vec[i].getNTimes();

        QLineSeries *series = new QLineSeries();
        series->append(i+1, startTime);
        series->append(i+1, endTime);
        series->setName(mac.toUpper());
        chart->addSeries(series);
        connect(series, &QLineSeries::clicked, this, &MainWindow::on_history_series_click);
        QPen pen = series->pen();
        pen.setWidth(5);
        series->setPen(pen);
    }
}

void MainWindow::on_history_series_click(QPointF point){
    int x = roundf(point.x()) - 1;
    qDebug() << "CLICKED SERIES WITH X " << x;

    ui->label_16->setText(historical_vector[x].getMacAddress().toUpper());

    QDateTime start;
    start.setTime_t(historical_vector[x].getStartTimestamp().toInt());
    ui->label_17->setText(start.toString(Qt::SystemLocaleDate));

    QDateTime end;
    end.setTime_t(historical_vector[x].getEndTimestamp().toInt());
    ui->label_19->setText(end.toString(Qt::SystemLocaleDate));

    ui->label_20->setText(QString::number(historical_vector[x].getNTimes()));
}

// FRANK ADD FUNCTIONs
void MainWindow::SetMutexsAndCondVars(void) {
    // FRANK ADD f()
    qDebug() << "setup mutexs and cond variables";
    this->notfied.reset(new bool());
    this->mutex.reset(new QMutex());
    this->waitCondition.reset(new QWaitCondition());

    this->restart.reset(new bool());
    this->mutex2.reset(new QMutex());
    this->waitCondition2.reset(new QWaitCondition());

    this->workerTab_One = nullptr;
}
void MainWindow::ManageTab1(int curr_tab) {

    // FRANK ADD f()

    if (curr_tab == 1) {
        //qDebug() << "manage tab one: run thread";
        *(this->notfied) = false;
        *(this->restart) = true;
        if(this->workerTab_One == nullptr) {
            qDebug() << "manage tab one: create thread";
            this->workerTab_One = new WorkerThreadTab(this->notfied, this->mutex, this->waitCondition, this->restart, this->mutex2, this->waitCondition2);
            //qRegisterMetaType<QList<QPair<QString, double>>>("<QList<QPair<QString, double>>>");
            connect(this->workerTab_One, &WorkerThreadTab::signal_data_ready,
                    this, &MainWindow::makePlotTab_One);
            qDebug() << "manage tab one: launch thread";
            this->workerTab_One->start();
            //qDebug() << "temporal diagram thread created and started";
        } else {
            //qDebug() << "try restart thread temporal diagram";
            qDebug() << "manage tab one: resume thread";
            {//std::unique_lock<QMutex> _lock(*this->mutex2); //_lock.lock();
                this->mutex2->lock();
                *(this->restart) = true;
                this->waitCondition2->wakeAll();
                this->mutex2->unlock();
            }
        }
    } else if ( this->old_tab == 1) {
        qDebug() << "manage tab one: pause thread";
        if (*this->restart == true) {
            qDebug() << "manage tab one: notify - pause";
            //std::unique_lock<QMutex> lock(*this->mutex);
            //lock.lock();

            this->mutex->lock();
            *(this->notfied) = true;
            *(this->restart) = false;
            this->waitCondition->wakeAll();
            this->mutex->unlock();
            //qDebug() << "manage tab one: notified";

        }
    }
}
void MainWindow::makePlotTab_One(QList<QPair<QString, double>> *List) {
    QChart *chart = new QChart();
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    if(List == nullptr) {
        return;
    }

    QLineSeries *series = new QLineSeries();   
    //unsigned long now = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1000);
    //std::chrono::seconds now = std::chrono::seconds(std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1000)-20);
    int i = 1;
    for(; i < List->size(); i++){
        int reference = List->at(0).second;
        int time = List->at(i).first.toInt();
        series->append((time - reference)%300, List->at(i).second);
    }
    series->setName("Devices");
    series->setBrush(QBrush("red"));
    chart->addSeries(series);
    chart->createDefaultAxes();

    //chart->axisX()->setRange(0, (int)now - List->at(0).second);
    chart->axisX()->setRange(0, 300);
    chart->axisY()->setRange(0, 10);
    ui->graphicsView_4->setChart(chart);
    ui->graphicsView_4->setStyleSheet("background-color: rgb(255, 255, 255)}");
    delete List;
}


void MainWindow::on_movements_devices_click(){
    QDateTime start_timestamp = ui->dateTimeEdit_3->dateTime();
    QDateTime end_timestamp = ui->dateTimeEdit_4->dateTime();

    this->movements_timestamp_start = QString::number(start_timestamp.toTime_t());
    this->movements_timestamp_end = QString::number(end_timestamp.toTime_t());

    /* Here I have to put the code to retrieve infos about devices and timestamps */
    hist_thread = new Historical_thread(QString::number(start_timestamp.toTime_t()), QString::number(end_timestamp.toTime_t()), 1);
    //qRegisterMetaType<QVector<Historical_device>>("QVector<Historical_device>");
    connect(hist_thread, &Historical_thread::devicesListSignal, this, &MainWindow::listDevicesSlot);
    hist_thread->start();
}


void MainWindow::listDevicesSlot(QStringList list){
//    this->devicesList = list;
//    for(int i = 0; i < list.size(); i++)
//        qDebug() << "DEVICE: " << list[i];
    ui->comboBox->clear();
    ui->comboBox->addItems(list);
}

void MainWindow::combobox_changed_slot(QString device){
    qDebug() << "DEVICE: " << device;
    if(device.isEmpty() || !QString::compare(device, "-")) return;

    /* Else I have to paint the point on the chart */
    hist_thread = new Historical_thread(movements_timestamp_start, movements_timestamp_end, 2);
    connect(hist_thread, &Historical_thread::devicePositionsSignal, this, &MainWindow::devicesPositionsSlot);
    hist_thread->setMacAddress(device);
    qRegisterMetaType<QVector<QPointF>>("QVector<QPointF>");
    hist_thread->start();
}

void MainWindow::devicesPositionsSlot(QVector<QPointF> vec){
    this->devicePositions = vec;
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(vec.size()-1);
    ui->horizontalSlider->setValue(0);
    disconnect(ui->horizontalSlider, &QSlider::sliderMoved, this, &MainWindow::on_slider_movement);
    connect(ui->horizontalSlider, &QSlider::sliderMoved, this, &MainWindow::on_slider_movement);
}

void MainWindow::on_slider_movement(int value){

    QChart *chart = new QChart();
    chart->setTheme(QChart::ChartThemeBlueCerulean);

    QScatterSeries *series = new QScatterSeries();

    series->append(devicePositions[value]);

    series->setPen(QPen(3));
    series->setName("Device");
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axisX()->setRange(X_START, X_END);
    chart->axisY()->setRange(Y_START, Y_END);
    ui->graphicsView_3->setChart(chart);
    ui->graphicsView_3->setStyleSheet("background-color: rgb(255, 255, 255)}");    
}
