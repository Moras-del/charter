#include "mainwindow.h"
#include "tinyexpr.h"

#include <QPushButton>
#include <QVBoxLayout>

double computeDerivative(double &x, te_expr *function, double delta);

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    minRange = new QLineEdit;
    maxRange = new QLineEdit;
    functionEdit = new QLineEdit;
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
    int min = minRange->text().toInt();
    int max = maxRange->text().toInt();
    QString expression = functionEdit->text();
    double x;
    int err;
    te_variable var = {"x", &x};
    te_expr *expr = te_compile(expression.toUtf8().data(), &var, 1, &err);

    QLineSeries *series = new QLineSeries;
    series->setName(expression);
    QLineSeries *derivativeSeries;
    if(derivativeEnabled->isChecked())
    {
        derivativeSeries = new QLineSeries;
        derivativeSeries->setName("derivative");
    }

    if(expr)
    {
        for(x = min; x <= max; x += 0.001)
        {
            *series << QPointF(x, te_eval(expr));
            if(derivativeEnabled->isChecked())
                *derivativeSeries << QPointF(x, computeDerivative(x, expr, 0.0001));
        }
        chart->addSeries(series);
        if(derivativeEnabled->isChecked())
            chart->addSeries(derivativeSeries);
        te_free(expr);
    }
    chart->createDefaultAxes();
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









