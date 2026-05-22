#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.resize(800, 600);
    window.setWindowTitle("Dashboard Demo");
    window.show();
    return app.exec();
}