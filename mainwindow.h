#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chartview.h"

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
    void handleCompute();
private:
    void computeFunction(QPair<double, double> range, double interval, QString expression, bool derivative, double* result);
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
