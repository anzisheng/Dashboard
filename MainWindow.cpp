#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QRandomGenerator>
#include <QMessageBox>
#include <QInputDialog>

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
{
    ui->setupUi(this);
    bool b = InitInstance();


    // 初始化压力表（0-40 MPa）
    ui->gaugeSpeed->setRange(0, 40);
    ui->gaugeSpeed->setUnit("MPa");
    ui->gaugeSpeed->setGaugeColor(QColor(70, 130, 200));
    ui->gaugeSpeed->setNeedleColor(QColor(255, 80, 80));

    // 初始化液位表（0-25 MPa，这里单位仍为MPa，可理解为液位当量）
    ui->gaugeRpm->setRange(0, 25);
    ui->gaugeRpm->setUnit("MPa");
    ui->gaugeRpm->setGaugeColor(QColor(100, 180, 100));
    ui->gaugeRpm->setNeedleColor(QColor(255, 120, 50));

    // 初始数值
    ui->gaugeSpeed->setValue(0);
    ui->gaugeRpm->setValue(0);
    ui->speedValueLabel->setText("0 MPa");
    ui->rpmValueLabel->setText("0 MPa");
    ui->statusLabel->setText("正常");
    ui->statusLabel->setStyleSheet("color: green;");

    // 连接手动更新按钮
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::on_updateButton_clicked);
    
    
    connect(ui->checkBox, &QCheckBox::toggled, this, &MainWindow::onCheckBoxToggled);

    // 手动连接"读取液位"按钮
   /* connect(ui->readLevelButton, &QPushButton::clicked,
       this, &MainWindow::on_readLevelButton_clicked);*/ 
    connect(ui->readPressureButton, &QPushButton::clicked,
        this, &MainWindow::on_readPressureButton_clicked);
    //setPressureButton
    connect(ui->setPressureButton, &QPushButton::clicked,
            this, &MainWindow::on_setPressureButton_clicked);
    // 启动定时器，每秒自动更新随机数据
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::updateInfo);
    m_timer->start(1000);
}
void MainWindow::onCheckBoxToggled(bool checked)
{
    m_bCheckTimer = checked;
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_updateButton_clicked()
{
    updateInfo();
}

void MainWindow::updateInfo()
{
    int newPressure = QRandomGenerator::global()->bounded(0, 41);   // 0-40
    int newLevel = QRandomGenerator::global()->bounded(0, 26);   // 0-25

    ui->gaugeSpeed->setValue(newPressure);
    ui->gaugeRpm->setValue(newLevel);

    ui->speedValueLabel->setText(QString("%1 MPa").arg(newPressure));
    ui->rpmValueLabel->setText(QString("%1 MPa").arg(newLevel));

    // 根据压力更新状态
    if (newPressure > 35) {
        ui->statusLabel->setText("超压警告！");
        ui->statusLabel->setStyleSheet("color: red; font-weight: bold;");
    }
    else if (newPressure > 25) {
        ui->statusLabel->setText("高压运行");
        ui->statusLabel->setStyleSheet("color: orange;");
    }
    else if (newPressure == 0) {
        ui->statusLabel->setText("无压停机");
        ui->statusLabel->setStyleSheet("color: gray;");
    }
    else {
        ui->statusLabel->setText("正常运行");
        ui->statusLabel->setStyleSheet("color: green;");
    }
}
//读取压力工作参数，包括告警值，PID参数
void MainWindow::on_readPressureButton_clicked()
{
    double pressure = ui->gaugeSpeed->value();
    QMessageBox::information(this, tr("压力读数"),
        tr("当前压力为 %1 MPa").arg(pressure, 0, 'f', 1));
        // TODO: Add your control notification handler code here
   if (m_bCheckTimer)
        PacketType = 0x04;
    else
        Rdpressworkpara();

}



void MainWindow::Rdpressworkpara()
{
	unsigned int i;
	unsigned char t = 0;
	bool flag = 0;
	unsigned char cmd_r;

	unsigned char temp = 0;

	unsigned int r_len;		//接收函数的返回值

	SOCKADDR_IN clientAddr;
	int nClientLen = sizeof(clientAddr);

	ZeroMemory(t_buf, BUF_SZIE);	//不包括12个空格的
	ZeroMemory(buf, BUF_SZIE);		//包含12个空格的

	t_buf[0] = 'E';		//包头，四个字节
	t_buf[1] = 'F';
	t_buf[2] = 'H';
	t_buf[3] = '1';

	t_buf[4] = 0x04;		//CMD，读取压力工作参数

	t = 0;
	for (i = 0; i < 5; i++)
		t = t + t_buf[i];
	t_buf[5] = t & 0xff;			//包校验

	//只可以写单个模块的单个通道
	CservAddr.sin_addr.S_un.S_un_b.s_b4 = DEFAULT_IP3;

	//发送
	if (sendto(sss, t_buf, 6, 0, (SOCKADDR*)&CservAddr, nServAddlen) == SOCKET_ERROR)
	{
		//	printf("recvfrom() failed: %d\n", WSAGetLastError());
		//	closesocket(sss);	//关闭套接字
		//	WSACleanup();		//释放套接字资源
		//	return 1;
	}

	Sleep(10);		//等待10毫秒，经过验证，延时1毫秒也工作正常

	i = 0;
	flag = 0;
	do {
		//BUF_SZIE是最大接收包长度，如果小于过来的数据包长度，会出错
		//recvfrom好像是个阻塞的函数
		i++;
		r_len = recvfrom(sss, buf, BUF_SZIE, 0, (SOCKADDR*)&clientAddr, &nClientLen);
		if (SOCKET_ERROR == r_len)
		{
			//	printf("recvfrom() failed: %d\n", WSAGetLastError());
			//	closesocket(sss);	//关闭套接字
			//	WSACleanup();	//释放套接字资源
			//	return 1;
		}
		else
		{
			//不传递参数，用全局变量，简单
			flag = 1;
		}
	} while ((i <= 10000) && (flag == 0));

	if (flag)
	{
		ZeroMemory(r_buf, BUF_SZIE);		//清空

		for (i = 0; i < r_len; i++)
			r_buf[i] = buf[i];

		t = 0;
		for (i = 0; i < (r_len - 1); i++)  //计算校验 
		{
			t = t + r_buf[i];
		}
		temp = r_buf[r_len - 1];
		cmd_r = r_buf[4];

		if ((temp == t) && (cmd_r == 0x04))	//检验校验，命令代码正确
		{
			//回读压力工作参数
			Fconverter.b[3] = r_buf[5];
			Fconverter.b[2] = r_buf[6];
			Fconverter.b[1] = r_buf[7];
			Fconverter.b[0] = r_buf[8];
			m_fPressAlmH = Fconverter.f;

			Fconverter.b[3] = r_buf[9];
			Fconverter.b[2] = r_buf[10];
			Fconverter.b[1] = r_buf[11];
			Fconverter.b[0] = r_buf[12];
			m_fPressAlmL = Fconverter.f;

			Fconverter.b[3] = r_buf[13];
			Fconverter.b[2] = r_buf[14];
			Fconverter.b[1] = r_buf[15];
			Fconverter.b[0] = r_buf[16];
			m_fPidParaP = Fconverter.f;

			Fconverter.b[3] = r_buf[17];
			Fconverter.b[2] = r_buf[18];
			Fconverter.b[1] = r_buf[19];
			Fconverter.b[0] = r_buf[20];
			m_fPidParaI = Fconverter.f;

			Fconverter.b[3] = r_buf[21];
			Fconverter.b[2] = r_buf[22];
			Fconverter.b[1] = r_buf[23];
			Fconverter.b[0] = r_buf[24];
			m_fPidParaD = Fconverter.f;

			m_fltEditPressAlmH = m_fPressAlmH;
			m_fltEditPressAlmL = m_fPressAlmL;

			m_fltEditParaP = m_fPidParaP;
			m_fltEditParaI = m_fPidParaI;
			m_fltEditParaD = m_fPidParaD;
			//UpdateData(FALSE);
		}
		return;	//正确
	}
	else
	{
		//	CString str;
		//	str.Format("下位机回应数据包超时！");
		//	AfxMessageBox(str.GetBuffer(str.GetLength()));
		//	m_intEditErr++;		//统计
		//UpdateData(FALSE);
		return;	//错误
	}
}

void MainWindow::on_setPressureButton_clicked()
{
    bool ok;
    double newPressure = QInputDialog::getDouble(this, tr("设置压力"),
        tr("请输入压力值 (0-40 MPa):"),
        ui->gaugeSpeed->value(),
        0, 40, 1, &ok);
    if (ok) {
        ui->gaugeSpeed->setValue(newPressure);
        ui->speedValueLabel->setText(QString("%1 MPa").arg(newPressure, 0, 'f', 1));

        // 更新状态标签
        if (newPressure > 35) {
            ui->statusLabel->setText("超压警告！");
            ui->statusLabel->setStyleSheet("color: red; font-weight: bold;");
        }
        else if (newPressure > 25) {
            ui->statusLabel->setText("高压运行");
            ui->statusLabel->setStyleSheet("color: orange;");
        }
        else if (newPressure == 0) {
            ui->statusLabel->setText("无压停机");
            ui->statusLabel->setStyleSheet("color: gray;");
        }
        else {
            ui->statusLabel->setText("正常运行");
            ui->statusLabel->setStyleSheet("color: green;");
        }
    }
    if (m_bCheckTimer)
        PacketType = 0x03;
    else
        Wrpressworkpara();
}

void MainWindow::Wrpressworkpara()
{
    unsigned int i;
    unsigned char t = 0;
    BOOL flag = 0;

    unsigned char temp = 0;

    ZeroMemory(t_buf, BUF_SZIE);	//不包括12个空格的

   // UpdateData(TRUE);

    t_buf[0] = 'E';		//包头，四个字节
    t_buf[1] = 'F';
    t_buf[2] = 'H';
    t_buf[3] = '1';

    t_buf[4] = 0x03;		//CMD，设置压力工作参数

    m_fPressAlmH = m_fltEditPressAlmH;
    m_fPressAlmL = m_fltEditPressAlmL;

    m_fPidParaP = m_fltEditParaP;
    m_fPidParaI = m_fltEditParaI;
    m_fPidParaD = m_fltEditParaD;

    //压力高限报警阈值
    Fconverter.f = m_fPressAlmH;
    t_buf[5] = Fconverter.b[3];		//32位数据，四个字节浮点数
    t_buf[6] = Fconverter.b[2];		//32位数据，四个字节浮点数
    t_buf[7] = Fconverter.b[1];		//32位数据，四个字节浮点数
    t_buf[8] = Fconverter.b[0];		//32位数据，四个字节浮点数

    //压力低限报警阈值
    Fconverter.f = m_fPressAlmL;
    t_buf[9] = Fconverter.b[3];		//32位数据，四个字节浮点数
    t_buf[10] = Fconverter.b[2];		//32位数据，四个字节浮点数
    t_buf[11] = Fconverter.b[1];		//32位数据，四个字节浮点数
    t_buf[12] = Fconverter.b[0];		//32位数据，四个字节浮点数

    //PID-P
    Fconverter.f = m_fPidParaP;
    t_buf[13] = Fconverter.b[3];		//32位数据，四个字节浮点数
    t_buf[14] = Fconverter.b[2];		//32位数据，四个字节浮点数
    t_buf[15] = Fconverter.b[1];		//32位数据，四个字节浮点数
    t_buf[16] = Fconverter.b[0];		//32位数据，四个字节浮点数

    //PID-I
    Fconverter.f = m_fPidParaI;
    t_buf[17] = Fconverter.b[3];		//32位数据，四个字节浮点数
    t_buf[18] = Fconverter.b[2];		//32位数据，四个字节浮点数
    t_buf[19] = Fconverter.b[1];		//32位数据，四个字节浮点数
    t_buf[20] = Fconverter.b[0];		//32位数据，四个字节浮点数

    //PID-D
    Fconverter.f = m_fPidParaD;
    t_buf[21] = Fconverter.b[3];		//32位数据，四个字节浮点数
    t_buf[22] = Fconverter.b[2];		//32位数据，四个字节浮点数
    t_buf[23] = Fconverter.b[1];		//32位数据，四个字节浮点数
    t_buf[24] = Fconverter.b[0];		//32位数据，四个字节浮点数

    t = 0;
    for (i = 0; i < 25; i++)
        t = t + t_buf[i];
    t_buf[25] = t & 0xff;			//包校验

    //只可以写单个模块的单个通道
    CservAddr.sin_addr.S_un.S_un_b.s_b4 = DEFAULT_IP3;

    //发送
    if (sendto(sss, t_buf, 26, 0, (SOCKADDR*)&CservAddr, nServAddlen) == SOCKET_ERROR)
    {
    }
}
void MainWindow::on_readLevelButton_clicked()
{
    double level = ui->gaugeRpm->value();
    QMessageBox::information(this, tr("液位读数"),
        tr("当前液位为 %1 MPa").arg(level, 0, 'f', 1));
    // TODO: Add your control notification handler code here
    if (m_bCheckTimer)
        PacketType = 0x02;
    else
        Rdywpara();
}

void MainWindow::Wrywpara()
{
    unsigned int i;
    unsigned char t = 0;
    BOOL flag = 0;

    unsigned char temp = 0;

    ZeroMemory(t_buf, BUF_SZIE);	//不包括12个空格的

    //UpdateData(TRUE);

    t_buf[0] = 'E';		//包头，四个字节
    t_buf[1] = 'F';
    t_buf[2] = 'H';
    t_buf[3] = '1';

    t_buf[4] = 0x01;		//CMD，设置液位参数

    m_fYwAlmH = m_fltEditYwAlmH;
    m_fYwAlmL = m_fltEditYwAlmL;
    m_fYwWorkH = m_fltEditYwWorkH;
    m_fYwWorkL = m_fltEditYwWorkL;

    //液位高限报警阈值
    Fconverter.f = m_fYwAlmH;
    t_buf[5] = Fconverter.b[3];		//32位数据，四个字节浮点数
    t_buf[6] = Fconverter.b[2];		//32位数据，四个字节浮点数
    t_buf[7] = Fconverter.b[1];		//32位数据，四个字节浮点数
    t_buf[8] = Fconverter.b[0];		//32位数据，四个字节浮点数

    //液位低限报警阈值
    Fconverter.f = m_fYwAlmL;
    t_buf[9] = Fconverter.b[3];		//32位数据，四个字节浮点数
    t_buf[10] = Fconverter.b[2];		//32位数据，四个字节浮点数
    t_buf[11] = Fconverter.b[1];		//32位数据，四个字节浮点数
    t_buf[12] = Fconverter.b[0];		//32位数据，四个字节浮点数

    //液位高限停止值
    Fconverter.f = m_fYwWorkH;
    t_buf[13] = Fconverter.b[3];		//32位数据，四个字节浮点数
    t_buf[14] = Fconverter.b[2];		//32位数据，四个字节浮点数
    t_buf[15] = Fconverter.b[1];		//32位数据，四个字节浮点数
    t_buf[16] = Fconverter.b[0];		//32位数据，四个字节浮点数

    //液位低限启动值
    Fconverter.f = m_fYwWorkL;
    t_buf[17] = Fconverter.b[3];		//32位数据，四个字节浮点数
    t_buf[18] = Fconverter.b[2];		//32位数据，四个字节浮点数
    t_buf[19] = Fconverter.b[1];		//32位数据，四个字节浮点数
    t_buf[20] = Fconverter.b[0];		//32位数据，四个字节浮点数

    //定时补液的状态
    t_buf[21] = m_bCheckAutoOilEnable;	//

    t = 0;
    for (i = 0; i < 22; i++)
        t = t + t_buf[i];
    t_buf[22] = t & 0xff;			//包校验

    //只可以写单个模块的单个通道
    CservAddr.sin_addr.S_un.S_un_b.s_b4 = DEFAULT_IP3;

    //发送
    if (sendto(sss, t_buf, 23, 0, (SOCKADDR*)&CservAddr, nServAddlen) == SOCKET_ERROR)
    {
    }

}

//读取液位设置参数
void MainWindow::Rdywpara()
{
    // TODO: Add your control notification handler code here
    unsigned int i;
    unsigned char t = 0;
    BOOL flag = 0;
    unsigned char cmd_r;

    unsigned char temp = 0;

    unsigned int r_len;		//接收函数的返回值

    SOCKADDR_IN clientAddr;
    int nClientLen = sizeof(clientAddr);

    ZeroMemory(t_buf, BUF_SZIE);	//不包括12个空格的
    ZeroMemory(buf, BUF_SZIE);		//包含12个空格的

    t_buf[0] = 'E';		//包头，四个字节
    t_buf[1] = 'F';
    t_buf[2] = 'H';
    t_buf[3] = '1';

    t_buf[4] = 0x02;		//CMD，读取液位设置参数

    t = 0;
    for (i = 0; i < 5; i++)
        t = t + t_buf[i];
    t_buf[5] = t & 0xff;			//包校验

    //只可以写单个模块的单个通道
    CservAddr.sin_addr.S_un.S_un_b.s_b4 = DEFAULT_IP3;

    //发送
    if (sendto(sss, t_buf, 6, 0, (SOCKADDR*)&CservAddr, nServAddlen) == SOCKET_ERROR)
    {
        //	printf("recvfrom() failed: %d\n", WSAGetLastError());
        //	closesocket(sss);	//关闭套接字
        //	WSACleanup();		//释放套接字资源
        //	return 1;
    }

    Sleep(10);		//等待10毫秒，经过验证，延时1毫秒也工作正常

    i = 0;
    flag = 0;
    do {
        //BUF_SZIE是最大接收包长度，如果小于过来的数据包长度，会出错
        //recvfrom好像是个阻塞的函数
        i++;
        r_len = recvfrom(sss, buf, BUF_SZIE, 0, (SOCKADDR*)&clientAddr, &nClientLen);
        if (SOCKET_ERROR == r_len)
        {
            //	printf("recvfrom() failed: %d\n", WSAGetLastError());
            //	closesocket(sss);	//关闭套接字
            //	WSACleanup();	//释放套接字资源
            //	return 1;
        }
        else
        {
            //不传递参数，用全局变量，简单
            flag = 1;
        }
    } while ((i <= 10000) && (flag == 0));

    if (flag)
    {
        ZeroMemory(r_buf, BUF_SZIE);		//清空

        for (i = 0; i < r_len; i++)
            r_buf[i] = buf[i];

        t = 0;
        for (i = 0; i < (r_len - 1); i++)  //计算校验 
        {
            t = t + r_buf[i];
        }
        temp = r_buf[r_len - 1];
        cmd_r = r_buf[4];

        if ((temp == t) && (cmd_r == 0x02))	//检验校验，命令代码正确
        {
            //液位部分，r_buf[7]-r_buf[10]是液位的传感器电流，不需要显示
            Fconverter.b[3] = r_buf[5];		//32位数据，四个字节浮点数
            Fconverter.b[2] = r_buf[6];		//32位数据，四个字节浮点数
            Fconverter.b[1] = r_buf[7];		//32位数据，四个字节浮点数
            Fconverter.b[0] = r_buf[8];		//32位数据，四个字节浮点数
            m_fYwAlmH = Fconverter.f;

            Fconverter.b[3] = r_buf[9];		//32位数据，四个字节浮点数
            Fconverter.b[2] = r_buf[10];	//32位数据，四个字节浮点数
            Fconverter.b[1] = r_buf[11];	//32位数据，四个字节浮点数
            Fconverter.b[0] = r_buf[12];	//32位数据，四个字节浮点数
            m_fYwAlmL = Fconverter.f;

            Fconverter.b[3] = r_buf[13];	//32位数据，四个字节浮点数
            Fconverter.b[2] = r_buf[14];	//32位数据，四个字节浮点数
            Fconverter.b[1] = r_buf[15];	//32位数据，四个字节浮点数
            Fconverter.b[0] = r_buf[16];	//32位数据，四个字节浮点数
            m_fYwWorkH = Fconverter.f;

            Fconverter.b[3] = r_buf[17];	//32位数据，四个字节浮点数
            Fconverter.b[2] = r_buf[18];	//32位数据，四个字节浮点数
            Fconverter.b[1] = r_buf[19];	//32位数据，四个字节浮点数
            Fconverter.b[0] = r_buf[20];	//32位数据，四个字节浮点数
            m_fYwWorkL = Fconverter.f;

            m_fltEditYwAlmH = m_fYwAlmH;
            m_fltEditYwAlmL = m_fYwAlmL;
            m_fltEditYwWorkH = m_fYwWorkH;
            m_fltEditYwWorkL = m_fYwWorkL;

            if (r_buf[21])
                m_bCheckAutoOilEnable = 1;
            else
                m_bCheckAutoOilEnable = 0;

           //UpdateData(FALSE);
        }
        return;	//正确
    }
    else
    {
        //	CString str;
        //	str.Format("下位机回应数据包超时！");
        //	AfxMessageBox(str.GetBuffer(str.GetLength()));
        //	m_intEditErr++;		//统计
//        UpdateData(FALSE);
        return;	//错误
    }
}


void MainWindow::on_setLevelButton_clicked()
{
    bool ok;
    double newLevel = QInputDialog::getDouble(this, tr("设置液位"),
        tr("请输入液位值 (0-25 MPa):"),
        ui->gaugeRpm->value(),
        0, 25, 1, &ok);
    if (ok) {
        ui->gaugeRpm->setValue(newLevel);
        ui->rpmValueLabel->setText(QString("%1 MPa").arg(newLevel, 0, 'f', 1));

        // 液位过高提醒（可选）
        if (newLevel > 22) {
            ui->statusLabel->setText("low！");
            ui->statusLabel->setStyleSheet("color: red; font-weight: bold;");
        }
        else if (newLevel > 18) {
            ui->statusLabel->setText("high");
            ui->statusLabel->setStyleSheet("color: orange;");
        }
        else {
            // 如果压力状态更重要，可以根据压力状态覆盖，这里简单处理
            if (ui->gaugeSpeed->value() > 0) {
                ui->statusLabel->setText("ywzc");
                ui->statusLabel->setStyleSheet("color: green;");
            }
        }
    }
    // TODO: Add your control notification handler code here
    if (m_bCheckTimer)
        PacketType = 0x01;
    else
        Wrywpara();

}