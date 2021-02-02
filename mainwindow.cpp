#include "mainwindow.h"
#include "tinyexpr.h"

#include <QPushButton>
#include <QVBoxLayout>

double computeDerivative(double &x, te_expr *function, double delta);

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    minRange = new QLineEdit("-10");
    minRange->setPlaceholderText("min range");
    maxRange = new QLineEdit("10");
    maxRange->setPlaceholderText("max range");
    functionEdit = new QLineEdit;
    functionEdit->setPlaceholderText("f(x)");
    derivativeEnabled = new QCheckBox("derivative");
    button = new QPushButton("Compute!");
    chart = new QChart;
    chartView = new ChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    setWidgets();
    connect(button, SIGNAL(clicked()), this, SLOT(handleCompute()));
}

MainWindow::~MainWindow()
{
    delete minRange;
    delete maxRange;
    delete functionEdit;
    delete button;
    delete chart;
    delete chartView;
}

void MainWindow::setWidgets()
{
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(minRange);
    hLayout->addWidget(maxRange);
    hLayout->addWidget(functionEdit);
    hLayout->addWidget(button);
    hLayout->addWidget(derivativeEnabled);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(hLayout);
    layout->addWidget(chartView);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);
}

void MainWindow::handleCompute()
{
    chart->removeAllSeries();
    double min = minRange->text().isEmpty()? -10 : minRange->text().toDouble();
    double max = maxRange->text().isEmpty()? -10 : maxRange->text().toDouble();
    QString expression = functionEdit->text();
    int size = (max-min)/0.001 + 1;
    if(derivativeEnabled->isChecked())
    {
        size *= 2;
        size++;
    }
    double result[size];
    double *ptr = result;
    computeFunction(qMakePair(min, max), 0.001, expression, derivativeEnabled->isChecked(), result);
    QLineSeries *series = new QLineSeries;
    series->setName(expression);
    QLineSeries *derivativeSeries;
    if(derivativeEnabled->isChecked())
    {
        derivativeSeries = new QLineSeries;
        derivativeSeries->setName("derivative");
    }
    for(double i = min; i <= max; i += 0.001)
    {
        *series << QPointF(i, *(ptr++));
        if(derivativeEnabled->isChecked())
            *derivativeSeries << QPointF(i, *(ptr++));
    }
    chart->addSeries(series);
    if(derivativeEnabled->isChecked())
        chart->addSeries(derivativeSeries);
    chart->createDefaultAxes();
}

void MainWindow::computeFunction(QPair<double, double> range, double interval, QString expression, bool derivative, double *result)
{
    double x;
    int err;
    te_variable var = {"x", &x};
    te_expr *expr = te_compile(expression.toUtf8().data(), &var, 1, &err);
    if(expr)
    {
        for(x = range.first; x <= range.second; x += interval)
        {
            auto val = te_eval(expr);
            *(result++) = val;
            if(derivative)
                *(result++) = computeDerivative(x, expr, 0.0001);
        }
    }
    te_free(expr);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        button->click();
    }
}

double computeDerivative(double &x, te_expr *function, double delta)
{
    double result1 = te_eval(function);
    x += delta;
    double result2 = te_eval(function);
    x -=delta;
    return (result2-result1)/delta;
}









