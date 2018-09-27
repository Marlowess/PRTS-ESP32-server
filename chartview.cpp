#include "chartview.h"
#include <QtCore/QtMath>
#include <QtCore/QDebug>

QT_CHARTS_USE_NAMESPACE

ChartView::ChartView(QWidget *parent)
    : QChartView(new QChart(), parent),
      m_scatter(nullptr),
      m_scatter2(nullptr){
    setRenderHint(QPainter::Antialiasing);

    chart()->setTitle("Devices radar");

    m_scatter = new QScatterSeries();
    m_scatter->setName("Devices");
    for (qreal x(1.0); x <= 10.0; x += 1.0) {
        for (qreal y(1.0); y <= 10.0; y += 1.0)
            *m_scatter << QPointF(x, y);
    }
    m_scatter2 = new QScatterSeries();
    m_scatter2->setName("Boards");

    chart()->addSeries(m_scatter2);
    chart()->addSeries(m_scatter);
    chart()->createDefaultAxes();
    chart()->axisX()->setRange(0, 10.0);
    chart()->axisY()->setRange(0, 10.0);



    connect(m_scatter, &QScatterSeries::clicked, this, &ChartView::handleClickedPoint);
}

ChartView::~ChartView(){}

void ChartView::clearDevices(){
    m_scatter->clear();
}

void ChartView::handleClickedPoint(const QPointF &point)
{
    QPointF clickedPoint = point;
    // Find the closest point from series 1
    QPointF closest(INT_MAX, INT_MAX);
    qreal distance(INT_MAX);
    const auto points = m_scatter->points();
    for (const QPointF &currentPoint : points) {
        qreal currentDistance = qSqrt((currentPoint.x() - clickedPoint.x())
                                      * (currentPoint.x() - clickedPoint.x())
                                      + (currentPoint.y() - clickedPoint.y())
                                      * (currentPoint.y() - clickedPoint.y()));
        if (currentDistance < distance) {
            distance = currentDistance;
            closest = currentPoint;
        }
    }

    // Remove the closes point from series 1 and append it to series 2
    m_scatter->remove(closest);
    m_scatter2->append(closest);
}
