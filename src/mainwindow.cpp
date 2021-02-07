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
    calculator = new Calculator(range, 0.001, expression);
    QList<QPointF> data = calculator->compute(Calculator::FUNCTION);
    addSeries(data, "f(x)");
    if(derivativeEnabled->isChecked())
    {
        data = calculator->compute(Calculator::DERIVATIVE);
        addSeries(data, "f'(x)");
    }
    chart->createDefaultAxes();
    delete calculator;
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
