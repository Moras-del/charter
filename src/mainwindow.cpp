#include "mainwindow.h"

#include <QPushButton>
#include <QVBoxLayout>

double computeDerivative(double &x, te_expr *function, double delta);

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    minRange = new QLineEdit("-10");
    maxRange = new QLineEdit("10");
    functionEdit = new QLineEdit;
    derivativeEnabled = new QCheckBox("derivative");
    button = new QPushButton("Compute!");
    chart = new QChart;
    chartView = new ChartView(chart);

    minRange->setPlaceholderText("min range");
    maxRange->setPlaceholderText("max range");
    functionEdit->setPlaceholderText("f(x)");
    chartView->setRenderHint(QPainter::Antialiasing);

    setWidgets();
    connect(button, SIGNAL(clicked()), this, SLOT(handleButton()));
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

void MainWindow::handleButton()
{
    chart->removeAllSeries();
    double min = minRange->text().isEmpty()? -10 : minRange->text().toDouble();
    double max = maxRange->text().isEmpty()? -10 : maxRange->text().toDouble();
    QPair<double, double> range = qMakePair(min, max);
    QString expression = functionEdit->text();
    QList<QPointF> data = compute(range, 0.001, expression, computeFunction);
    addSeries(data, "f(x)");
    if(derivativeEnabled->isChecked())
    {
        data = compute(range, 0.001, expression, computeDerivative);
        addSeries(data, "f'(x)");
    }
    chart->createDefaultAxes();
}

QList<QPointF> MainWindow::compute(QPair<double, double> range, double interval, QString expression, double (*computer)(double &x, te_expr *func))
{
    QList<QPointF> result;
    double x;
    int err;
    te_variable var = {"x", &x};
    te_expr *expr = te_compile(expression.toUtf8().data(), &var, 1, &err);
    if(expr)
    {
        for(x = range.first; x <= range.second; x += interval)
        {
            double y = computer(x, expr);
            result.push_back({x, y});
        }
    }
    te_free(expr);
    return result;
}

double MainWindow::computeFunction(double &x, te_expr *function)
{
    return te_eval(function);
}

double MainWindow::computeDerivative(double &x, te_expr *function)
{
    double delta = 0.0001;
    double result1 = te_eval(function);
    x += delta;
    double result2 = te_eval(function);
    x -=delta;
    return (result2-result1)/delta;
}

void MainWindow::addSeries(const QList<QPointF>& data, const QString& name)
{
    QLineSeries *series = new QLineSeries;
    series->setName(name);
    series->append(data);
    chart->addSeries(series);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
        button->click();
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
