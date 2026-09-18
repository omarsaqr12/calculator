#include "../src/calculator_app.h"

#include <QApplication>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <cmath>
#include <iostream>
#include <stdexcept>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    calculator window;
    setupCalculatorControls(window);
    CalculatorKeyFilter filter(window);
    app.installEventFilter(&filter);
    window.show();
    window.activateWindow();
    app.processEvents();

    auto *display = window.findChild<QLineEdit *>("res");
    if (!display || !display->isReadOnly()) {
        std::cerr << "FAIL: read-only display missing\n";
        return 1;
    }
    auto press = [&](const char *name) {
        auto *button = window.findChild<QPushButton *>(name);
        if (!button || !button->isVisible())
            throw std::runtime_error(std::string("Missing or hidden button: ") + name);
        if (!window.centralWidget()->rect().contains(button->geometry()))
            throw std::runtime_error(std::string("Button outside window: ") + name);
        button->click();
    };
    auto check = [&](const char *label, double expected) {
        bool ok = false;
        const double actual = display->text().toDouble(&ok);
        if (!ok || !std::isfinite(actual) || std::abs(actual - expected) > 1e-10)
            throw std::runtime_error(std::string(label) + ": got " + display->text().toStdString());
    };
    auto reset = [&]() { press("clearAll"); };
    try {
        reset(); press("b0"); press("b5"); check("zero then five", 5);
        reset(); press("b1"); press("decimal"); press("b5"); check("decimal", 1.5);
        press("backspace"); check("backspace", 1);
        press("clearEntry"); check("clear entry", 0);

        reset(); press("b9"); press("memoryStore"); reset(); press("memoryRecall");
        check("memory recall", 9);
        press("memoryAdd"); press("memoryRecall"); check("memory add", 18);
        press("memoryClear"); press("memoryRecall"); check("memory clear", 0);

        reset(); press("b5"); press("factorial"); check("5 factorial", 120);
        reset(); press("b9"); press("squareRoot"); check("square root", 3);
        reset(); press("b5"); press("b0"); press("percent"); check("percent", .5);
        reset(); press("b3"); press("b0"); press("sin"); check("sine degrees", .5);
        reset(); press("b1"); press("log"); check("base 10 log", 0);
        reset(); press("b1"); press("ln"); check("natural log", 0);
        reset(); press("b0"); press("exp"); check("exponential", 1);
        reset(); press("b2"); press("diffrentiate"); press("b3"); press("eq");
        check("power-rule derivative of x^3 at 2", 12);
        reset(); press("b2"); press("integ"); press("b1"); press("eq");
        check("integral of x from 0 to 2", 2);

        reset();
        if (!window.isActiveWindow())
            throw std::runtime_error("Offscreen window was not activated for keyboard test");
        auto key = [&](int code, const QString &text) {
            QKeyEvent event(QEvent::KeyPress, code, Qt::NoModifier, text);
            QCoreApplication::sendEvent(display, &event);
            if (!event.isAccepted())
                throw std::runtime_error("Keyboard event was not handled");
        };
        key(Qt::Key_1, "1"); key(Qt::Key_Plus, "+"); key(Qt::Key_2, "2");
        key(Qt::Key_Return, "\r"); check("keyboard 1+2", 3);
        key(Qt::Key_Escape, ""); check("keyboard escape", 0);
    } catch (const std::exception &error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
    std::cout << "PASS: added controls, memory, scientific functions, power-rule calculus and keyboard\n";
    return 0;
}
