#include "../src/calculator.h"

#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <iostream>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    calculator window;
    auto *display = window.findChild<QLineEdit *>("res");
    auto *one = window.findChild<QPushButton *>("b1");
    auto *two = window.findChild<QPushButton *>("b2");
    auto *plus = window.findChild<QPushButton *>("ad");
    auto *times = window.findChild<QPushButton *>("mult");
    auto *equals = window.findChild<QPushButton *>("eq");
    if (!display || !one || !two || !plus || !times || !equals)
    {
        std::cerr << "Missing a required calculator widget\n";
        return 1;
    }

    one->click();
    plus->click();
    two->click();
    equals->click();
    if (display->text() != QStringLiteral("3"))
    {
        std::cerr << "1 + 2 should equal 3; got " << display->text().toStdString() << '\n';
        return 1;
    }
    times->click();
    two->click();
    equals->click();
    if (display->text() != QStringLiteral("6"))
    {
        std::cerr << "3 * 2 should equal 6; got " << display->text().toStdString() << '\n';
        return 1;
    }
    std::cout << "PASS: 1 + 2 = 3; 3 * 2 = 6\n";
    return 0;
}
