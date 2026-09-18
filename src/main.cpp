#include "calculator_app.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    calculator window;
    setupCalculatorControls(window);
    CalculatorKeyFilter keyFilter(window);
    app.installEventFilter(&keyFilter);
    window.show();
    return app.exec();
}
