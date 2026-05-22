#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_updateButton_clicked();
    void updateInfo();
    void on_startStopButton_clicked();   // 启动/停止按钮槽函数
    void on_stopCoastButton_clicked();   // 停机/回油按钮槽函数

private:
    Ui::MainWindow* ui;
    QTimer* m_timer;
    bool m_timerRunning;   // 记录定时器状态
};

#endif