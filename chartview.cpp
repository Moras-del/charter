#include "chartview.h"

void ChartView::mousePressEvent(QMouseEvent *event)
{
    switch(event->button())
    {
    case Qt::RightButton:
        chart()->zoomReset();
        break;
    case Qt::LeftButton:
        offset = event->pos();
        break;
    }
}

void ChartView::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
    }
}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        auto dPos = event->pos() - offset;
        chart()->scroll(-dPos.x(), dPos.y());
        offset = event->pos();
        QApplication::restoreOverrideCursor();
    } else
    {
        const QPointF widgetPos = event->localPos();
        auto const scenePos = mapToScene(QPoint(static_cast<int>(widgetPos.x()), static_cast<int>(widgetPos.y())));
        const QPointF chartPos = chart()->mapFromScene(scenePos);
        const QPointF pos = chart()->mapToValue(chartPos);
        QString title("X = ");
        title.append(QString::number(pos.x(), 'f', 2))
                .append("  Y = ")
                .append(QString::number(pos.y(), 'f', 2));
        chart()->setTitle(title);
    }
    QChartView::mouseMoveEvent(event);
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    int delta = event->delta();
    float scaleFactor = (delta > 0)? 1.1 : 0.9;
    chart()->zoom(scaleFactor);
}
