#include <QString>
#include <QPointF>
#include <QPair>
#include <QList>
#include "../include/tinyexpr.h"
#ifndef CALCULATOR_H
#define CALCULATOR_H


class Calculator
{
private:
    QString expression;
    QPair<double, double> range;
    double interval;
    static double computeDerivative(double &x, te_expr* func);
    static double computeFunction(double &x, te_expr* func);
public:
    enum type {FUNCTION, DERIVATIVE};
    Calculator(QPair<double, double> range, double interval, QString _expression);
    QList<QPointF> compute(type funcType);
};

#endif // CALCULATOR_H
