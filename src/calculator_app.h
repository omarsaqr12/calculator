#ifndef CALCULATOR_APP_H
#define CALCULATOR_APP_H

#include "calculator.h"

#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

// Keep the original calculator core and wire its existing slots to visible controls.
// Its number/operator slots depend on QObject::sender(), so keyboard input
// must click the corresponding button rather than invoke a slot directly.
inline void addCalculatorControl(calculator &window, const QString &text,
                                 const char *name, int x, int y, const char *slot)
{
    auto *button = new QPushButton(text, window.centralWidget());
    button->setObjectName(QString::fromLatin1(name));
    button->setGeometry(x, y, 81, 25);
    QObject::connect(button, SIGNAL(released()), &window, slot);
    button->show();
}

inline void setupCalculatorControls(calculator &window)
{
    if (auto *display = window.findChild<QLineEdit *>("res"))
        display->setReadOnly(true);

    addCalculatorControl(window, "0", "b0", 0, 170, SLOT(NumPressed()));
    addCalculatorControl(window, ".", "decimal", 90, 170, SLOT(decimalPressed()));
    addCalculatorControl(window, "CE", "clearEntry", 180, 170, SLOT(clearPressed()));
    addCalculatorControl(window, "AC", "clearAll", 270, 170, SLOT(clearAllPressed()));
    addCalculatorControl(window, "Back", "backspace", 360, 170, SLOT(backspacePressed()));

    addCalculatorControl(window, "MS", "memoryStore", 0, 205, SLOT(memoryStorePressed()));
    addCalculatorControl(window, "MR", "memoryRecall", 90, 205, SLOT(memoryRecallPressed()));
    addCalculatorControl(window, "MC", "memoryClear", 180, 205, SLOT(memoryClearPressed()));
    addCalculatorControl(window, "M+", "memoryAdd", 270, 205, SLOT(memoryAddPressed()));
    addCalculatorControl(window, "M-", "memorySubtract", 360, 205, SLOT(memorySubtractPressed()));

    addCalculatorControl(window, "sqrt", "squareRoot", 0, 240, SLOT(squareRootPressed()));
    addCalculatorControl(window, "%", "percent", 90, 240, SLOT(percentagePressed()));
    addCalculatorControl(window, "n!", "factorial", 180, 240, SLOT(factorialPressed()));
    addCalculatorControl(window, "sin", "sin", 270, 240, SLOT(sinPressed()));
    addCalculatorControl(window, "cos", "cos", 360, 240, SLOT(cosPressed()));
    addCalculatorControl(window, "tan", "tan", 0, 275, SLOT(tanPressed()));
    addCalculatorControl(window, "log", "log", 90, 275, SLOT(logPressed()));
    addCalculatorControl(window, "ln", "ln", 180, 275, SLOT(lnPressed()));
    addCalculatorControl(window, "exp", "exp", 270, 275, SLOT(expPressed()));
    // The existing powerPressed implementation is incomplete: no power button.
}

class CalculatorKeyFilter : public QObject
{
public:
    explicit CalculatorKeyFilter(calculator &window) : window_(window) {}

protected:
    bool eventFilter(QObject *target, QEvent *event) override
    {
        if (event->type() != QEvent::KeyPress || !window_.isActiveWindow())
            return QObject::eventFilter(target, event);
        auto *widget = qobject_cast<QWidget *>(target);
        if (widget != &window_ && (!widget || !window_.isAncestorOf(widget)))
            return QObject::eventFilter(target, event);
        auto *key = static_cast<QKeyEvent *>(event);
        const char *button = nullptr;
        if (key->key() >= Qt::Key_0 && key->key() <= Qt::Key_9)
        {
            const QString name = "b" + QString::number(key->key() - Qt::Key_0);
            if (auto *digit = window_.findChild<QPushButton *>(name))
            {
                digit->click();
                return true;
            }
        }
        switch (key->key())
        {
        case Qt::Key_Plus: button = "ad"; break;
        case Qt::Key_Minus: button = "subs"; break;
        case Qt::Key_Asterisk: button = "mult"; break;
        case Qt::Key_Slash: button = "div"; break;
        case Qt::Key_Equal:
        case Qt::Key_Enter:
        case Qt::Key_Return: button = "eq"; break;
        case Qt::Key_Period: button = "decimal"; break;
        case Qt::Key_Backspace: button = "backspace"; break;
        case Qt::Key_Delete: button = "clearEntry"; break;
        case Qt::Key_Escape: button = "clearAll"; break;
        default: break;
        }
        if (button)
        {
            if (auto *control = window_.findChild<QPushButton *>(button))
            {
                control->click();
                return true;
            }
        }
        return QObject::eventFilter(target, event);
    }

private:
    calculator &window_;
};

#endif // CALCULATOR_APP_H
