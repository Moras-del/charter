#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "calculator.h"
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
    void addSeries(const QList<QPointF>& data, const QString& name);
    void setWidgets();
    QLineEdit *minRange;
    QLineEdit *maxRange;
    QLineEdit *functionEdit;
    QCheckBox *derivativeEnabled;
    QPushButton *button;
    QChart *chart;
    QChartView *chartView;
    Calculator *calculator;
};

#endif // MAINWINDOW_H
