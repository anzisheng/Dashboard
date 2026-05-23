#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
// 在这里定义全局的变量、常量、函数
#define DEFAULT_IP0		192
#define DEFAULT_IP1		168
#define DEFAULT_IP2		1
#define DEFAULT_IP3		205

#define  BUF_SZIE	 800

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void Rdpressworkpara();

private slots:
    void on_updateButton_clicked();
    void updateInfo();
    void on_readPressureButton_clicked();
    void on_setPressureButton_clicked();
    void on_readLevelButton_clicked();
    void on_setLevelButton_clicked();

private:
    Ui::MainWindow* ui;
    QTimer* m_timer;
    unsigned char PacketType;
};

#endif