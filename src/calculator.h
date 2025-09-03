
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class calculator;
}
QT_END_NAMESPACE

struct CalculationEntry
{
    QString expression;
    double result;
};

class calculator : public QMainWindow
{
    Q_OBJECT

public:
    calculator(QWidget *parent = nullptr);
    ~calculator();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::calculator *ui;

    // Memory operations
    double memoryValue;

    // History
    QVector<CalculationEntry> calculationHistory;

    // Helper functions
    double factorial(int n);
    double power(double base, double exponent);
    double squareRoot(double value);
    double percentage(double value, double percent);
    double differentiate(double x, const QString &function);
    double integrate(double a, double b, const QString &function);
    bool isValidNumber(const QString &text);
    void addToHistory(const QString &expression, double result);
    void updateDisplay(double value);
    void clearAll();
    void clearEntry();
    void backspace();

private slots:
    void NumPressed();
    void operbutpressed();
    void equpressed();
    void decimalPressed();
    void clearPressed();
    void clearAllPressed();
    void backspacePressed();
    void memoryStorePressed();
    void memoryRecallPressed();
    void memoryClearPressed();
    void memoryAddPressed();
    void memorySubtractPressed();
    void squareRootPressed();
    void powerPressed();
    void percentagePressed();
    void factorialPressed();
    void sinPressed();
    void cosPressed();
    void tanPressed();
    void logPressed();
    void lnPressed();
    void expPressed();
};

#endif // CALCULATOR_H
