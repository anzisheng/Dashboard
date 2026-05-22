#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_timer(nullptr)
    , m_timerRunning(true)   // 初始为运行状态
{
    ui->setupUi(this);

    // 压力表（左侧）
ui->gaugeSpeed->setRange(0, 40);      // 假设压力范围 0~40 MPa
ui->gaugeSpeed->setUnit("MPa");
ui->gaugeSpeed->setGaugeColor(QColor(70, 130, 200));
ui->gaugeSpeed->setNeedleColor(QColor(255, 80, 80));


 // 液压表（右侧）
ui->gaugeRpm->setRange(0, 25);        // 假设液压范围 0~25 MPa
ui->gaugeRpm->setUnit("MPa");
ui->gaugeRpm->setGaugeColor(QColor(100, 180, 100));
ui->gaugeRpm->setNeedleColor(QColor(255, 120, 50));

    // 初始数值
    ui->gaugeSpeed->setValue(0);
    ui->gaugeRpm->setValue(0);

    ui->speedValueLabel->setText("0 km/h");
    ui->rpmValueLabel->setText("0 RPM");
    ui->statusLabel->setText("停车");

    // 连接按钮信号
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::on_updateButton_clicked);
    connect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::on_startStopButton_clicked);
    connect(ui->stopCoastButton, &QPushButton::clicked, this, &MainWindow::on_stopCoastButton_clicked);

    // 启动定时器（每秒更新）
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::updateInfo);
    m_timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_updateButton_clicked()
{
    // 手动触发更新
    updateInfo();
}

void MainWindow::updateInfo()
{
    // 如果定时器停止，则不生成随机数（也可以保留手动更新，但停机状态建议不更新）
    if (!m_timerRunning)
        return;

 int newSpeed = QRandomGenerator::global()->bounded(0, 41);   // 0~40
int newRpm = QRandomGenerator::global()->bounded(0, 26);  // 0~25

    ui->gaugeSpeed->setValue(newSpeed);
    ui->gaugeRpm->setValue(newRpm);

    ui->speedValueLabel->setText(QString("%1 km/h").arg(newSpeed));
    ui->rpmValueLabel->setText(QString("%1 RPM").arg(newRpm));

    if (newSpeed > 180) {
        ui->statusLabel->setText("超速警告！");
        ui->statusLabel->setStyleSheet("color: red; font-weight: bold;");
    }
    else if (newSpeed > 120) {
        ui->statusLabel->setText("高速行驶");
        ui->statusLabel->setStyleSheet("color: orange;");
    }
    else if (newSpeed == 0) {
        ui->statusLabel->setText("停车");
        ui->statusLabel->setStyleSheet("color: gray;");
    }
    else {
        ui->statusLabel->setText("正常行驶");
        ui->statusLabel->setStyleSheet("color: green;");
    }
}

void MainWindow::on_startStopButton_clicked()
{
    if (m_timerRunning) {
        // 当前正在运行，停止定时器
        m_timer->stop();
        m_timerRunning = false;
        ui->startStopButton->setText("启动");
        ui->statusLabel->setText("已停止");
        ui->statusLabel->setStyleSheet("color: orange;");
    }
    else {
        // 当前停止，启动定时器
        m_timer->start(1000);
        m_timerRunning = true;
        ui->startStopButton->setText("停止");
        ui->statusLabel->setText("运行中");
        ui->statusLabel->setStyleSheet("color: green;");
        updateInfo(); // 立即更新一次
    }
}

void MainWindow::on_stopCoastButton_clicked()
{
    // 停机/回油：仪表归零，停止定时器，设置状态为“停机”
    ui->gaugeSpeed->setValue(0);
    ui->gaugeRpm->setValue(0);
    ui->speedValueLabel->setText("0 km/h");
    ui->rpmValueLabel->setText("0 RPM");

    // 如果定时器在运行，则停止它
    if (m_timerRunning) {
        m_timer->stop();
        m_timerRunning = false;
        ui->startStopButton->setText("启动");
    }

    ui->statusLabel->setText("stop/return");
    ui->statusLabel->setStyleSheet("color: red; font-weight: bold;");
}