#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QRandomGenerator>
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_timer(nullptr)
{
    ui->setupUi(this);

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

    // 启动定时器，每秒自动更新随机数据
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

void MainWindow::on_readPressureButton_clicked()
{
    /*double pressure = ui->gaugeSpeed->value();
    QMessageBox::information(this, tr("压力读数"),
        tr("当前压力为 %1 MPa").arg(pressure, 0, 'f', 1));*/

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
			UpdateData(FALSE);
		}
		return;	//正确
	}
	else
	{
		//	CString str;
		//	str.Format("下位机回应数据包超时！");
		//	AfxMessageBox(str.GetBuffer(str.GetLength()));
		//	m_intEditErr++;		//统计
		UpdateData(FALSE);
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
}

void MainWindow::on_readLevelButton_clicked()
{
    double level = ui->gaugeRpm->value();
    QMessageBox::information(this, tr("液位读数"),
        tr("当前液位为 %1 MPa").arg(level, 0, 'f', 1));
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
}