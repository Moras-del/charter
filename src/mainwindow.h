#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chartview.h"
#include "../include/tinyexpr.h"
#include <QtCharts>
#include <QLabel>
#include <QLineEdit>
#include <QLineSeries>
#include <QMainWindow>
#include <QPushButton>

using namespace QtCharts;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    virtual void keyPressEvent(QKeyEvent *event) override;
private slots:
    void handleButton();
private:
    QList<QPointF> compute(QPair<double, double> range, double interval, QString expression, double (*computer)(double &x, te_expr *func));
    static double computeFunction(double &x, te_expr *function);
    static double computeDerivative(double &x, te_expr* function);
    void addSeries(const QList<QPointF>& data, const QString& name);
    void setWidgets();
    QLineEdit *minRange;
    QLineEdit *maxRange;
    QLineEdit *functionEdit;
    QCheckBox *derivativeEnabled;
    QPushButton *button;
    QChart *chart;
    QChartView *chartView;
};

#endif // MAINWINDOW_H
