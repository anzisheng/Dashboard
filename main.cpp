#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.resize(800, 600);
    window.setWindowTitle("火主机控制系统");

    window.setWindowState(window.windowState() ^ Qt::WindowFullScreen);
    window.show();
    return app.exec();
}