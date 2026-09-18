#include "calculator.h"

#include <QApplication>
#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

// Preserve the original calculator implementation and expose its existing slots.
// NumPressed and operbutpressed depend on QObject::sender(), so keyboard input
// must click the corresponding button rather than call those slots directly.
static void addControl(calculator &window, const QString &text, const char *name,
                       int x, int y, const char *slot)
{
    auto *button = new QPushButton(text, window.centralWidget());
    button->setObjectName(QString::fromLatin1(name));
    button->setGeometry(x, y, 81, 25);
    QObject::connect(button, SIGNAL(released()), &window, slot);
    button->show();
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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    calculator window;
    if (auto *display = window.findChild<QLineEdit *>("res"))
        display->setReadOnly(true);

    // The original .ui omits these controls even though their slots exist.
    addControl(window, "0", "b0", 0, 170, SLOT(NumPressed()));
    addControl(window, ".", "decimal", 90, 170, SLOT(decimalPressed()));
    addControl(window, "CE", "clearEntry", 180, 170, SLOT(clearPressed()));
    addControl(window, "AC", "clearAll", 270, 170, SLOT(clearAllPressed()));
    addControl(window, "Back", "backspace", 360, 170, SLOT(backspacePressed()));

    addControl(window, "MS", "memoryStore", 0, 205, SLOT(memoryStorePressed()));
    addControl(window, "MR", "memoryRecall", 90, 205, SLOT(memoryRecallPressed()));
    addControl(window, "MC", "memoryClear", 180, 205, SLOT(memoryClearPressed()));
    addControl(window, "M+", "memoryAdd", 270, 205, SLOT(memoryAddPressed()));
    addControl(window, "M-", "memorySubtract", 360, 205, SLOT(memorySubtractPressed()));

    addControl(window, "sqrt", "squareRoot", 0, 240, SLOT(squareRootPressed()));
    addControl(window, "%", "percent", 90, 240, SLOT(percentagePressed()));
    addControl(window, "n!", "factorial", 180, 240, SLOT(factorialPressed()));
    addControl(window, "sin", "sin", 270, 240, SLOT(sinPressed()));
    addControl(window, "cos", "cos", 360, 240, SLOT(cosPressed()));
    addControl(window, "tan", "tan", 0, 275, SLOT(tanPressed()));
    addControl(window, "log", "log", 90, 275, SLOT(logPressed()));
    addControl(window, "ln", "ln", 180, 275, SLOT(lnPressed()));
    addControl(window, "exp", "exp", 270, 275, SLOT(expPressed()));
    // The existing powerPressed implementation is incomplete; do not expose it.

    CalculatorKeyFilter keyFilter(window);
    app.installEventFilter(&keyFilter);
    window.show();
    return app.exec();
}
