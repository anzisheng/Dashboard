
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QRandomGenerator>
#include <stdio.h>
#include <atlstr.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") // 链接 Winsock 库

SOCKET	sss;		//套接字

WSADATA		wsd;			//WSADATA变量
SOCKADDR_IN	CservAddr;		//下位机服务器地址
int	nServAddlen;
SOCKADDR_IN		servAddr;	//上位机监听服务器地址

char	t_buf[BUF_SZIE];	//发送数据缓冲区
char	buf[BUF_SZIE];		//发送数据缓冲区
unsigned char r_buf[BUF_SZIE];	//接收数据缓冲区
bool MainWindow::InitInstance()
{
    // *********************************************************************** 
// 下面开始设置监听下位机回传数据用的套接字
// *********************************************************************** 
    //初始化套结字动态库
   
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    {
        //	MessageBox("WSAStartup failed!");
        return false;
    }
    
    //申请套接字sss
    sss = socket(AF_INET, SOCK_DGRAM, 0);
    if (sss == INVALID_SOCKET)
    {
        char temp[10];
        CString str = "申请套接字失败＝";

        itoa(WSAGetLastError(), temp, 10);
        str = str + temp;
        //	MessageBox(str);
        WSACleanup();//释放套接字资源
        return FALSE;
    }
    
    int nErrCode;	//返回值
    int nBufLen;	//接收数据缓冲区大小
    int nOptlen = sizeof(nBufLen);

    //获取接收数据缓冲区大小
    nErrCode = getsockopt(sss, SOL_SOCKET, SO_RCVBUF, (char*)&nBufLen, &nOptlen);
    if (SOCKET_ERROR == nErrCode)
    {
        //处理失败
    }
    
    //设置接收数据缓冲区为原来的10倍
    nBufLen *= 10;
    nErrCode = setsockopt(sss, SOL_SOCKET, SO_RCVBUF, (char*)&nBufLen, nOptlen);
    if (SOCKET_ERROR == nErrCode)
    {
        //失败处理
    }
    
    //检查设置系统接收数据缓冲区是否成功
    int uiNewRcvBuf;
    getsockopt(sss, SOL_SOCKET, SO_RCVBUF, (char*)&uiNewRcvBuf, &nOptlen);
    if (SOCKET_ERROR == nErrCode || uiNewRcvBuf != nBufLen)
    {
        //失败处理
    }

    //服务器地址
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons((short)5001);			//端口多少合适？
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);	//IP
    
    //绑定
    if (bind(sss, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
    {
        char temp[10];
        CString str = "帮定失败＝";

        itoa(WSAGetLastError(), temp, 10);
        str = str + temp;
        //	MessageBox(str);

        closesocket(sss);	//关闭套接字
        WSACleanup();		//释放套接字资源
        return false;
    }
   
    //设定为非阻塞模式，在UDP下同样有效
    unsigned long ul = 1;
    int nRet = ioctlsocket(sss, FIONBIO, (unsigned long*)&ul);
    if (SOCKET_ERROR == nRet)
    {
        char temp[10];
        CString str = "设置非阻塞失败＝";

        itoa(WSAGetLastError(), temp, 10);
        str = str + temp;
        //	MessageBox(str);

        closesocket(sss);	//关闭套接字
        WSACleanup();		//释放套接字资源 
        return FALSE;
    } /**/
    // *********************************************************************** 
// 下面开始设置向下位机发送数据用的套接字
// *********************************************************************** 
    //下位机服务器地址
    CservAddr.sin_family = AF_INET;
    CservAddr.sin_addr.S_un.S_un_b.s_b1 = DEFAULT_IP0;
    CservAddr.sin_addr.S_un.S_un_b.s_b2 = DEFAULT_IP1;
    CservAddr.sin_addr.S_un.S_un_b.s_b3 = DEFAULT_IP2;
    CservAddr.sin_addr.S_un.S_un_b.s_b4 = DEFAULT_IP3;

    //对应服务器的端口号码，统一为6000
    CservAddr.sin_port = htons((short)6000);
    nServAddlen = sizeof(CservAddr);
    //	len=sizeof(SOCKADDR);

    return true;
}
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_timer(nullptr)
    , m_timerRunning(true)   // 初始为运行状态
{
    ui->setupUi(this);
    bool b = InitInstance();

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
    ui->statusLabel->setText(QStringLiteral("停止"));

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
        ui->statusLabel->setText(QStringLiteral("超速警告！"));
        ui->statusLabel->setStyleSheet("color: red; font-weight: bold;");
    }
    else if (newSpeed > 120) {
        ui->statusLabel->setText(QStringLiteral("高速行驶"));
        ui->statusLabel->setStyleSheet("color: orange;");
    }
    else if (newSpeed == 0) {
        ui->statusLabel->setText(QStringLiteral("停车"));
        ui->statusLabel->setStyleSheet("color: gray;");
    }
    else {
        ui->statusLabel->setText(QStringLiteral("正常行驶"));
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