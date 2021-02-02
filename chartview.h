#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts>

using namespace QtCharts;

class ChartView : public QChartView
{
    Q_OBJECT
public:
    ChartView(QChart *chart): QChartView(chart){}
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
private:
    QPoint offset;
};
#endif // CHARTVIEW_H
