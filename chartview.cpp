#include "chartview.h"
#include <QtCore/QtMath>
#include <QtCore/QDebug>

QT_CHARTS_USE_NAMESPACE

ChartView::ChartView(QWidget *parent)
    : QChartView(new QChart(), parent),
      m_scatter(nullptr),
      m_scatter2(nullptr){
    setRenderHint(QPainter::Antialiasing);

    //chart()->setTitle("Devices radar");

    m_scatter = new QScatterSeries();
    m_scatter->setName("Devices");
    for (qreal x(1.0); x < 10.0; x += 1.0) {
        for (qreal y(1.0); y < 10.0; y += 1.0)
            *m_scatter << QPointF(x, y);
    }
    m_scatter2 = new QScatterSeries();
    m_scatter2->setName("Boards");

    chart()->addSeries(m_scatter2);
    chart()->addSeries(m_scatter);
    chart()->createDefaultAxes();
    chart()->axisX()->setRange(0, 10.0);
    chart()->axisY()->setRange(0, 10.0);



    //connect(m_scatter, &QScatterSeries::clicked, this, &ChartView::handleClickedPoint);
}

ChartView::~ChartView(){}
