#include "calculator.h"
#include "ui_calculator.h"
#include <QtMath>
#include <QMessageBox>
#include <QDebug>

// Global variables
double CalcVal = 0;
bool division = false;
bool multi = false;
bool add = false;
bool subs = false;
bool diff = false;
bool inte = false;
bool powerOp = false;
bool waitingForOperand = false;
QString currentExpression = "";

calculator::calculator(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::calculator), memoryValue(0.0)
{
    ui->setupUi(this);
    ui->res->setText("0");

    // Connect number buttons (0-9)
    QPushButton *buttons[10];
    for (int i = 0; i < 10; i++)
    {
        QString name = "b" + QString::number(i);
        buttons[i] = findChild<QPushButton *>(name);
        if (buttons[i])
        {
            connect(buttons[i], SIGNAL(released()), this, SLOT(NumPressed()));
        }
    }

    // Connect operator buttons
    connect(ui->ad, SIGNAL(released()), this, SLOT(operbutpressed()));
    connect(ui->div, SIGNAL(released()), this, SLOT(operbutpressed()));
    connect(ui->mult, SIGNAL(released()), this, SLOT(operbutpressed()));
    connect(ui->subs, SIGNAL(released()), this, SLOT(operbutpressed()));
    connect(ui->eq, SIGNAL(released()), this, SLOT(equpressed()));

    // Connect calculus buttons (will implement properly)
    connect(ui->diffrentiate, SIGNAL(released()), this, SLOT(operbutpressed()));
    connect(ui->integ, SIGNAL(released()), this, SLOT(operbutpressed()));

    // Enable keyboard input
    setFocusPolicy(Qt::StrongFocus);
}

calculator::~calculator()
{
    delete ui;
}

void calculator::keyPressEvent(QKeyEvent *event)
{
    // Handle keyboard input
    switch (event->key())
    {
    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        // Simulate number button press
        {
            QString digit = QString::number(event->key() - Qt::Key_0);
            QString oldval = ui->res->text();
            if (oldval == "0" || waitingForOperand)
            {
                ui->res->setText(digit);
                waitingForOperand = false;
            }
            else
            {
                ui->res->setText(oldval + digit);
            }
        }
        break;
    case Qt::Key_Plus:
        add = true;
        operbutpressed();
        break;
    case Qt::Key_Minus:
        subs = true;
        operbutpressed();
        break;
    case Qt::Key_Asterisk:
        multi = true;
        operbutpressed();
        break;
    case Qt::Key_Slash:
        division = true;
        operbutpressed();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Equal:
        equpressed();
        break;
    case Qt::Key_Period:
        decimalPressed();
        break;
    case Qt::Key_Backspace:
        backspace();
        break;
    case Qt::Key_Escape:
        clearAll();
        break;
    case Qt::Key_Delete:
        clearEntry();
        break;
    }
    QMainWindow::keyPressEvent(event);
}

void calculator::NumPressed()
{
    QPushButton *button = (QPushButton *)sender();
    QString newval = button->text();
    QString oldval = ui->res->text();

    if (oldval == "0" || waitingForOperand)
    {
        ui->res->setText(newval);
        waitingForOperand = false;
    }
    else
    {
        ui->res->setText(oldval + newval);
    }
}

void calculator::operbutpressed()
{
    QPushButton *button = (QPushButton *)sender();
    QString buttonText = button->text();

    // Reset all operation flags
    division = multi = add = subs = diff = inte = powerOp = false;

    QString dis = ui->res->text();
    if (!isValidNumber(dis))
        return;

    CalcVal = dis.toDouble();
    currentExpression = dis + " " + buttonText + " ";

    // Set appropriate operation flag
    if (buttonText == "/")
        division = true;
    else if (buttonText == "*")
        multi = true;
    else if (buttonText == "+")
        add = true;
    else if (buttonText == "-")
        subs = true;
    else if (buttonText == "d/dx")
        diff = true;
    else if (buttonText == "integrate")
        inte = true;

    waitingForOperand = true;
}

void calculator::equpressed()
{
    QString cur = ui->res->text();
    if (!isValidNumber(cur))
        return;

    double curr = cur.toDouble();
    double solution = 0;
    bool validOperation = true;
    QString operation = "";

    try
    {
        if (add)
        {
            solution = CalcVal + curr;
            operation = "+";
        }
        else if (multi)
        {
            solution = CalcVal * curr;
            operation = "*";
        }
        else if (division)
        {
            if (curr == 0)
            {
                QMessageBox::warning(this, "Error", "Cannot divide by zero!");
                return;
            }
            solution = CalcVal / curr;
            operation = "/";
        }
        else if (subs)
        {
            solution = CalcVal - curr;
            operation = "-";
        }
        else if (diff)
        {
            // Simple polynomial differentiation: d/dx(ax^n) = n*ax^(n-1)
            // For simplicity, assume we're differentiating x^curr at point CalcVal
            if (curr == 0)
            {
                solution = 0; // d/dx(constant) = 0
            }
            else
            {
                solution = curr * qPow(CalcVal, curr - 1); // d/dx(x^n) = n*x^(n-1)
            }
            operation = "d/dx";
        }
        else if (inte)
        {
            // Simple integration: ∫x^n dx = x^(n+1)/(n+1)
            // Integrate x^curr from 0 to CalcVal
            if (curr == -1)
            {
                QMessageBox::warning(this, "Error", "Cannot integrate x^(-1) using this method!");
                return;
            }
            double antiderivative = qPow(CalcVal, curr + 1) / (curr + 1);
            solution = antiderivative; // Definite integral from 0 to CalcVal
            operation = "∫";
        }
        else
        {
            validOperation = false;
        }

        if (validOperation)
        {
            updateDisplay(solution);

            // Add to history
            QString expression = currentExpression + cur + " = " + QString::number(solution);
            addToHistory(expression, solution);

            // Reset for next calculation
            CalcVal = solution;
            waitingForOperand = true;

            // Reset operation flags
            division = multi = add = subs = diff = inte = powerOp = false;
        }
    }
    catch (...)
    {
        QMessageBox::warning(this, "Error", "Calculation error occurred!");
    }
}

void calculator::decimalPressed()
{
    QString current = ui->res->text();
    if (!current.contains('.'))
    {
        if (waitingForOperand)
        {
            ui->res->setText("0.");
            waitingForOperand = false;
        }
        else
        {
            ui->res->setText(current + ".");
        }
    }
}

void calculator::clearPressed()
{
    clearEntry();
}

void calculator::clearAllPressed()
{
    clearAll();
}

void calculator::backspacePressed()
{
    backspace();
}

void calculator::memoryStorePressed()
{
    QString current = ui->res->text();
    if (isValidNumber(current))
    {
        memoryValue = current.toDouble();
    }
}

void calculator::memoryRecallPressed()
{
    updateDisplay(memoryValue);
    waitingForOperand = false;
}

void calculator::memoryClearPressed()
{
    memoryValue = 0.0;
}

void calculator::memoryAddPressed()
{
    QString current = ui->res->text();
    if (isValidNumber(current))
    {
        memoryValue += current.toDouble();
    }
}

void calculator::memorySubtractPressed()
{
    QString current = ui->res->text();
    if (isValidNumber(current))
    {
        memoryValue -= current.toDouble();
    }
}

void calculator::squareRootPressed()
{
    QString current = ui->res->text();
    if (isValidNumber(current))
    {
        double value = current.toDouble();
        if (value < 0)
        {
            QMessageBox::warning(this, "Error", "Cannot calculate square root of negative number!");
            return;
        }
        double result = squareRoot(value);
        updateDisplay(result);
        addToHistory("√(" + current + ") = " + QString::number(result), result);
    }
}

void calculator::powerPressed()
{
    // This would be used for x^y operations
    powerOp = true;
    operbutpressed();
}

void calculator::percentagePressed()
{
    QString current = ui->res->text();
    if (isValidNumber(current))
    {
        double value = current.toDouble();
        double result = value / 100.0;
        updateDisplay(result);
        addToHistory(current + "% = " + QString::number(result), result);
    }
}

void calculator::factorialPressed()
{
    QString current = ui->res->text();
    if (isValidNumber(current))
    {
        double value = current.toDouble();
        if (value < 0 || value != (int)value || value > 170)
        {
            QMessageBox::warning(this, "Error", "Factorial only works for non-negative integers ≤ 170!");
            return;
        }
        double result = factorial((int)value);
        updateDisplay(result);
        addToHistory(current + "! = " + QString::number(result), result);
    }
}

void calculator::sinPressed()
{
    QString current = ui->res->text();
    if (isValidNumber(current))
    {
        double value = current.toDouble();
        double result = qSin(qDegreesToRadians(value)); // Assuming degrees
        updateDisplay(result);
        addToHistory("sin(" + current + "°) = " + QString::number(result), result);
    }
}

void calculator::cosPressed()
{
    QString current = ui->res->text();
    if (isValidNumber(current))
    {
        double value = current.toDouble();
        double result = qCos(qDegreesToRadians(value)); // Assuming degrees
        updateDisplay(result);
        addToHistory("cos(" + current + "°) = " + QString::number(result), result);
    }
}

void calculator::tanPressed()
{
    QString current = ui->res->text();
    if (isValidNumber(current))
    {
        double value = current.toDouble();
        double result = qTan(qDegreesToRadians(value)); // Assuming degrees
        updateDisplay(result);
        addToHistory("tan(" + current + "°) = " + QString::number(result), result);
    }
}

void calculator::logPressed()
{
    QString current = ui->res->text();
    if (isValidNumber(current))
    {
        double value = current.toDouble();
        if (value <= 0)
        {
            QMessageBox::warning(this, "Error", "Logarithm only works for positive numbers!");
            return;
        }
        double result = qLn(value) / qLn(10); // log base 10
        updateDisplay(result);
        addToHistory("log(" + current + ") = " + QString::number(result), result);
    }
}

void calculator::lnPressed()
{
    QString current = ui->res->text();
    if (isValidNumber(current))
    {
        double value = current.toDouble();
        if (value <= 0)
        {
            QMessageBox::warning(this, "Error", "Natural logarithm only works for positive numbers!");
            return;
        }
        double result = qLn(value);
        updateDisplay(result);
        addToHistory("ln(" + current + ") = " + QString::number(result), result);
    }
}

void calculator::expPressed()
{
    QString current = ui->res->text();
    if (isValidNumber(current))
    {
        double value = current.toDouble();
        double result = qExp(value);
        updateDisplay(result);
        addToHistory("e^(" + current + ") = " + QString::number(result), result);
    }
}

// Helper function implementations
double calculator::factorial(int n)
{
    if (n <= 1)
        return 1;
    double result = 1;
    for (int i = 2; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

double calculator::power(double base, double exponent)
{
    return qPow(base, exponent);
}

double calculator::squareRoot(double value)
{
    return qSqrt(value);
}

double calculator::percentage(double value, double percent)
{
    return value * percent / 100.0;
}

double calculator::differentiate(double x, const QString &function)
{
    // Simple numerical differentiation for basic functions
    // This is a simplified implementation
    Q_UNUSED(function);
    double h = 0.0001;
    // For x^2, derivative at x is 2x
    return 2 * x; // Simplified example
}

double calculator::integrate(double a, double b, const QString &function)
{
    // Simple numerical integration using trapezoidal rule
    Q_UNUSED(function);
    int n = 1000; // Number of intervals
    double h = (b - a) / n;
    double sum = 0;

    // For x^2 integration
    for (int i = 0; i <= n; i++)
    {
        double x = a + i * h;
        double y = x * x; // f(x) = x^2
        if (i == 0 || i == n)
        {
            sum += y;
        }
        else
        {
            sum += 2 * y;
        }
    }
    return (h / 2) * sum;
}

bool calculator::isValidNumber(const QString &text)
{
    bool ok;
    text.toDouble(&ok);
    return ok && !text.isEmpty();
}

void calculator::addToHistory(const QString &expression, double result)
{
    CalculationEntry entry;
    entry.expression = expression;
    entry.result = result;
    calculationHistory.append(entry);

    // Keep only last 100 calculations
    if (calculationHistory.size() > 100)
    {
        calculationHistory.removeFirst();
    }

    // Print to debug for now (since we're not changing UI)
    qDebug() << "History:" << expression;
}

void calculator::updateDisplay(double value)
{
    if (qIsInf(value))
    {
        ui->res->setText("∞");
    }
    else if (qIsNaN(value))
    {
        ui->res->setText("Error");
    }
    else
    {
        ui->res->setText(QString::number(value, 'g', 15));
    }
}

void calculator::clearAll()
{
    ui->res->setText("0");
    CalcVal = 0;
    division = multi = add = subs = diff = inte = powerOp = false;
    waitingForOperand = false;
    currentExpression = "";
}

void calculator::clearEntry()
{
    ui->res->setText("0");
    waitingForOperand = false;
}

void calculator::backspace()
{
    QString current = ui->res->text();
    if (current.length() > 1)
    {
        current.chop(1);
        ui->res->setText(current);
    }
    else
    {
        ui->res->setText("0");
    }
}
