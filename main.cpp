#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // 查找任务栏窗口
    HWND hTaskbar = ::FindWindow(L"Shell_TrayWnd", nullptr);
    if (hTaskbar) {
        // 隐藏任务栏
        ::ShowWindow(hTaskbar, SW_HIDE);
    }

    MainWindow window;
    //window.resize(800, 600);
    window.setWindowTitle("火主机控制系统");

    //window.setWindowState(window.windowState() ^ Qt::WindowFullScreen);
    window.show();
    return app.exec();
}