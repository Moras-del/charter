#include "calculator.h"

Calculator::Calculator(QPair<double, double> _range, double _interval, QString _expression)
    : range(_range),
    expression(_expression),
    interval(_interval) {}

QList<QPointF> Calculator::compute(Calculator::type funcType)
{
    auto computer = (funcType == FUNCTION)? computeFunction : computeDerivative;
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


double Calculator::computeDerivative(double &x, te_expr *func)
{
    double delta = 0.0001;
    double result1 = te_eval(func);
    x += delta;
    double result2 = te_eval(func);
    x -=delta;
    return (result2-result1)/delta;
}

double Calculator::computeFunction(double &x, te_expr *func)
{
    return te_eval(func);
}
