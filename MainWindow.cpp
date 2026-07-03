#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QRandomGenerator>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>
//#include <MyDialog.h>
#include "MyDialog.h"
#include "MyDialog_press.h"
#include "Dialog1.h"
#include "Dialog2.h"
#include "Dialog3.h"
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
    //if (sss == INVALID_SOCKET) 
    {
        int error_code = WSAGetLastError();
        printf("socket failed with error: %d\n", error_code);
    }
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

//void MainWindow::ReadData()
//{
//}
void MainWindow::SetPressWorkStatus()
{
    unsigned int i;
    unsigned char t = 0;
    BOOL flag = 0;

    unsigned char temp = 0;

    ZeroMemory(t_buf, BUF_SZIE);

    //UpdateData(TRUE);
    //m_fltEditPressureSet得到编辑框内的值 
    //m_fltEditPressureSet;
    QString Number_Two = ui->lineEdit_5->text();
    m_fPressureSet = Number_Two.toFloat();


    t_buf[0] = 'E';		//包头，四个字节
    t_buf[1] = 'F';
    t_buf[2] = 'H';
    t_buf[3] = '1';

    t_buf[4] = 0x05;		//CMD，设置压力工作状态

   // m_fPressureSet = m_fltEditPressureSet.get_float();
    //设定当前工作压力
    Fconverter.f = m_fPressureSet;
    t_buf[5] = Fconverter.b[3];		//32位数据，四个字节浮点数
    t_buf[6] = Fconverter.b[2];		//32位数据，四个字节浮点数
    t_buf[7] = Fconverter.b[1];		//32位数据，四个字节浮点数
    t_buf[8] = Fconverter.b[0];		//32位数据，四个字节浮点数

    t_buf[9] = WorkCmd;			//工作状态

    t = 0;
    for (i = 0; i < 10; i++)
        t = t + t_buf[i];
    t_buf[10] = t & 0xff;			//包校验

    //只可以写单个模块的单个通道
    CservAddr.sin_addr.S_un.S_un_b.s_b4 = DEFAULT_IP3;
    //发送
    int result = sendto(sss, t_buf, 11, 0, (SOCKADDR*)&CservAddr, nServAddlen);
    if (result == SOCKET_ERROR)
    {
        qDebug() << "发送错误";
    }

}
void MainWindow::ReadData()
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

    unsigned char DiTemp = 0;
    unsigned char DoTemp = 0;

    ZeroMemory(t_buf, BUF_SZIE);	//不包括12个空格的
    ZeroMemory(buf, BUF_SZIE);		//包含12个空格的

    t_buf[0] = 'E';		//包头，四个字节
    t_buf[1] = 'F';
    t_buf[2] = 'H';
    t_buf[3] = '1';

    t_buf[4] = 0x00;		//CMD，读取一次当前状态数据，DIDO、AI、压力液位、运行状态、变频器设置和当前频率，补液状态

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

   // Sleep(10);		//等待10毫秒，经过验证，延时1毫秒也工作正常

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

        if ((temp == t) && (cmd_r == 0x00))	//检验校验，命令代码正确
        {
            //DI部分
            DiTemp = r_buf[5];
            if (0x01 == (DiTemp & 0x01))	//急停输入
                m_bCheckStopIn = 1;
            else
                m_bCheckStopIn = 0;

            if (0x02 == (DiTemp & 0x02))	//过压开关输入
                m_bCheckOverPressure = 1;
            else
                m_bCheckOverPressure = 0;

            //DO部分
            DoTemp = r_buf[6];
            if (0x01 == (DoTemp & 0x01))	//泄压阀
                m_bCheckRelief = 1;
            else
                m_bCheckRelief = 0;

            if (0x02 == (DoTemp & 0x02))	//BEEP报警器
                m_bCheckBeep = 1;
            else
                m_bCheckBeep = 0;

            if (0x04 == (DoTemp & 0x04))	//补油泵
                m_bCheckPump = 1;
            else
                m_bCheckPump = 0;

            //液位部分，r_buf[7]-r_buf[10]是液位的传感器电流，不需要显示
            Fconverter.b[3] = r_buf[11];		//32位数据，四个字节浮点数
            Fconverter.b[2] = r_buf[12];		//32位数据，四个字节浮点数
            Fconverter.b[1] = r_buf[13];		//32位数据，四个字节浮点数
            Fconverter.b[0] = r_buf[14];		//32位数据，四个字节浮点数
            m_fltEditYwCurr = Fconverter.f;

            if (r_buf[15])
                m_strEditYwStatus = "补油中";
            else
                m_strEditYwStatus = "";

            //压力部分，r_buf[16]-r_buf[19]是压力的传感器电流，不需要显示
            Fconverter.b[3] = r_buf[20];		//32位数据，四个字节浮点数
            Fconverter.b[2] = r_buf[21];		//32位数据，四个字节浮点数
            Fconverter.b[1] = r_buf[22];		//32位数据，四个字节浮点数
            Fconverter.b[0] = r_buf[23];		//32位数据，四个字节浮点数
            m_fltEditPressureCurr = Fconverter.f;

         /*   m_fPressureSet = dialog1.m_pressSetEdit->text().toFloat();
            m_fltEditPressureCurr = m_fPressureSet;*/

            switch (r_buf[24])
            {
            case 0:
                m_intRadioWorkStatus = 0;		//待机
                ui->radioButton->setChecked(true);
                ui->radioButton_2->setChecked(false);
                ui->radioButton_3->setChecked(false);

                break;
            case 1:
                m_intRadioWorkStatus = 1;		//工作
                ui->radioButton_2->setChecked(true);
                ui->radioButton->setChecked(false);
                ui->radioButton_3->setChecked(false);
                break;
            case 2:
                m_intRadioWorkStatus = 2;		//泄压
                ui->radioButton_3->setChecked(true);
                ui->radioButton->setChecked(false);
                ui->radioButton_2->setChecked(false);
                break;
            }

            switch (r_buf[25])
            {
            case 0:
                m_strEditPressureAlarmStatus = "";
                break;
            case 1:
                m_strEditPressureAlarmStatus = "压力欠压告警";
                break;
            case 2:
                m_strEditPressureAlarmStatus = "压力过压告警";
                break;
            }

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



MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_timer(nullptr)
{
    // 设置窗口标题（标题栏将显示此文本）
    setWindowTitle("火主机控制系统");

    // 设置窗口固定大小为 800x600
    setFixedSize(800, 600);

    ui->setupUi(this);

    bool b = InitInstance();
    m_bCheckTimer = true;
    PacketType = 0X00;

    RandomNum = 10;

    WorkCmd = 0;		//进入待机状态
    ui->textEdit->append("test for clear as an error");

    //dialog = new MyDialog(this);
    //dialog1 = new MyDialog_press(this);

    // 压力表：范围 0-100，单位 MPa
    // 您可以自由调整角度范围：setAngleRange(起始角, 结束角) 顺时针方向
    // 例如：0在6点（270°），100在9点（180°）顺时针跨度为 270°（270->540）
    ui->gaugeSpeed->setAngleRange(270.0 - 45, 180.0-45);   // 注意：结束角小于起始角时会自动计算顺时针跨过0°的差值
    //ui->gaugeSpeed->setAngleRange(270.0, 450.0);
    ui->gaugeSpeed->setRange(0, 2.5);
    ui->gaugeSpeed->setUnit("MPa");
    ui->gaugeSpeed->setGaugeColor(QColor(70, 130, 200));
    ui->gaugeSpeed->setNeedleColor(QColor(255, 80, 80));

    // 液位表，同样设置
    ui->gaugeRpm->setAngleRange(270.0 - 45, 180.0 - 45);
    ui->gaugeRpm->setRange(0, 100);
    ui->gaugeRpm->setUnit("%");
    ui->gaugeRpm->setGaugeColor(QColor(100, 180, 100));
    ui->gaugeRpm->setNeedleColor(QColor(255, 120, 50));

    // 初始数值
    ui->gaugeSpeed->setValue(0);
    ui->gaugeRpm->setValue(0);
    //ui->speedValueLabel->setText("0 MPa");
    //ui->rpmValueLabel->setText("0 MPa");
//    ui->statusLabel->setText("正常");
//    ui->statusLabel->setStyleSheet("color: green;");
    m_bCheckAutoOilEnable = false;
    m_fltEditPressAlmH = 0.0f;
    //ui->speedValueLabel_2->setText(QString("%1 MPa").arg(m_fltEditPressAlmH));
    //ui->speedValueLabel_2->setText(QString("压力告警值: %1 MPa").arg(m_fltEditPressAlmH));
    //m_fltEditYwWorkL = 20.0f;
//    rpmValueLabel
    //ui->rpmValueLabel->setText(QString("%1 MPa").arg(m_fltEditYwWorkL));
    
	//m_fYwAlmH = 15.0f;
 //   ui->rpmValueLabel_2->setText(QString("%1 MPa").arg(m_fYwAlmH));
	//m_fYwAlmL = 0.0f;
 //   ui->rpmValueLabel_3->setText(QString("%1 MPa").arg(m_fYwAlmL));

    // 连接手动更新按钮
    connect(ui->updateButton_2, &QPushButton::clicked, this, &MainWindow::on_updateButton_clicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pressUpdateButton_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pressParaButton_2_clicked);
    //void on_pressParaButton_2_clicked();//pushButton_2
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::on_ywParaButton_5_clicked);

    ui->lineEdit_5->installEventFilter(this);
    
    //connect(ui->checkBox, &QCheckBox::toggled, this, &MainWindow::onCheckBoxToggled);

    // 手动连接"读取液位"按钮
   /* connect(ui->readLevelButton, &QPushButton::clicked,
       this, &MainWindow::on_readLevelButton_clicked);*/ 
    /*connect(ui->readPressureButton, &QPushButton::clicked,
        this, &MainWindow::on_readPressureButton_clicked);*/
    //setPressureButton
   /* connect(ui->setPressureButton, &QPushButton::clicked,
            this, &MainWindow::on_setPressureButton_clicked);*/
    connect(ui->start_stop,&QPushButton::clicked, this, &MainWindow::on_start_stopButton_clicked);

    //connect(ui->stopLoopoil,&QPushButton::clicked, this, &MainWindow::on_start_stopButton_clicked);
    connect(ui->StopLoopoil,&QPushButton::clicked, this, &MainWindow::on_StopLoopoilButton_clicked);

    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_MMXParaButton_4_clicked);

    //void on_MMXParaButton_4_clicked();//pushButton_4
    
    // 启动定时器，每秒自动更新随机数据
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::updateInfo);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::updateTime);
    //m_openButton = new QPushButton("设置液位参数", this);
    m_timer->start(1000);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    // 当 lineEdit 收到鼠标按下事件时，弹出数字键盘对话框
    if (obj == ui->lineEdit_5 && event->type() == QEvent::MouseButtonPress) {
        onLineEditClicked();
        return true;  // 事件已处理，不再传递（避免焦点变化）
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::onLineEditClicked()
{
    // 如果对话框还未创建，则创建；否则重复使用
    if (!m_dialog) {
        m_dialog = new NumPadDialog(this);
        // 连接对话框的 accepted 信号
        connect(m_dialog, &QDialog::accepted, this, &MainWindow::onDialogAccepted);
    }

    // 将当前输入框的内容设置为对话框的初始文本
    m_dialog->setText(ui->lineEdit_5->text());

    // 显示模态对话框
    m_dialog->exec();
}
void MainWindow::onDialogAccepted()
{
    // 对话框确认后，获取输入的新文本并设置到主编辑框
    QString newText = m_dialog->getText();
    ui->lineEdit_5->setText(newText);
}
void MainWindow::on_MMXParaButton_4_clicked()
{
	qDebug() << "on_MMXParaButton_4_clicked";
    Dialog3 dialog3(this);
    //if(m_bCheckStopIn == 1)
	 //   dialog3.m_bCheckStopIn->setChecked(m_bCheckStopIn);
  //  //else 
		////dialog3.m_bCheckStopIn->setChecked(false);
  //      dialog3.m_bCheckOverPressure->setChecked(m_bCheckOverPressure);
  //      dialog3.m_bCheckRelief->setChecked(m_bCheckRelief);
  //      dialog3.m_bCheckBeep->setChecked(m_bCheckBeep);
  //      dialog3.m_bCheckPump->setChecked(m_bCheckPump);



	dialog3.exec();

}
#include "DialogX.h"
void MainWindow::on_ywParaButton_5_clicked()
{
    DialogX dialogx(this);
    dialogx.exec();
    /*
    m_fYwAlmH = dialog2.m_fltEditYwAlmH->text().toFloat();
    m_fYwAlmL = dialog2.m_fltEditYwAlmL->text().toFloat();
    m_fYwWorkH = dialog2.m_fltEditYwWorkH->text().toFloat();
    m_fYwWorkL = dialog2.m_fltEditYwWorkL->text().toFloat();
    */

}
#include "GroupDialog.h"

void MainWindow::on_pressParaButton_2_clicked()
{
    GroupDialog dialog1(this);
	dialog1.exec();
    qDebug() << "hello";
    //m_fPressAlmH = dialog1.m_fltEditPressAlmH->text().toFloat();
    //m_fPressAlmL = dialog1.m_fltEditPressAlmL->text().toFloat();
    //m_fPidParaP = dialog1.m_fltEditParaP->text().toFloat();
    //m_fPidParaI = dialog1.m_fltEditParaI->text().toFloat();
    //m_fPidParaI = dialog1.m_fltEditParaI->text().toFloat();
    
}
void MainWindow::on_pressUpdateButton_clicked()
{

    qDebug() << "on_pressUpdateButton_clicked";
    QString Number_Two = ui->lineEdit_5->text();
    //m_fPressureSet = Number_Two.toFloat();
    m_fltEditPressureCurr = Number_Two.toFloat();
    ui->gaugeSpeed->setValue(m_fltEditPressureCurr);// = m_fPressureSet;

}
void MainWindow::updateTime()
{
    qDebug() << "定时器触发！";
    if (PacketType)
    {
        switch (PacketType)
        {
        case 0x01:
            Wrywpara();		//写液位设置参数
            break;

        case 0x02:
            Rdywpara();		//读液位设置参数
            break;

        case 0x03:			//写压力设置参数
            Wrpressworkpara();
            break;

        case 0x04:			//读压力设置参数
            Rdpressworkpara();
            break;

        case 0x05:
            SetPressWorkStatus();	//设定压力工作状态
        }

        PacketType = 0;
    }
    else
        ReadData();
    ui->gaugeSpeed->setValue(m_fltEditPressureCurr);
    m_vecPressures.push_back(m_fltEditPressureCurr);
	//qDebug() << "m_fltEditPressureCurr size : " << m_vecPressures.size();
    ui->gaugeRpm->setValue(m_fltEditYwCurr);
    
    // 在这里添加你需要周期性执行的任务，例如刷新界面
}
//ui->gaugeSpeed->setValue(m_fltEditPressureCurr);
//void MainWindow::onCheckBoxToggled(bool checked)
//{
//    m_bCheckTimer = checked;
//    if(m_bCheckTimer)
//        m_timer->start(1000);
//    else
//    {
//        m_timer->stop();
//    }
//}
MainWindow::~MainWindow()
{
    delete ui;
}

void clearLayout(QLayout* layout) {
    if (!layout) return;
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (item->widget()) {
            delete item->widget();
        }
        else if (item->layout()) {
            clearLayout(item->layout());
        }
        delete item;
    }
}

void MainWindow::on_updateButton_clicked()
{
    ui->textEdit->clear();
    //updateInfo();
    //QLayout* layout = &infoGroup.layout();
    //clearLayout(&infoGroup);
}

void MainWindow::updateInfo()
{
    double newPressure = QRandomGenerator::global()->bounded(2.5);
    double newLevel = QRandomGenerator::global()->bounded(2.5);

    ui->gaugeSpeed->setValue(newPressure);
    ui->gaugeRpm->setValue(newLevel);

//    ui->speedValueLabel->setText(QString("%1 MPa").arg(newPressure));
//    ui->rpmValueLabel->setText(QString("%1 MPa").arg(newLevel));

   /* if (newPressure > 85) {
        ui->statusLabel->setText("超压警告！");
        ui->statusLabel->setStyleSheet("color: red; font-weight: bold;");
    }
    else if (newPressure > 70) {
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
    }*/
}
//读取压力工作参数，包括告警值，PID参数
void MainWindow::on_readPressureButton_clicked()
{
    qDebug() << "enter into on_readPressureButton_clicked";
    double pressure = ui->gaugeSpeed->value();
   /* QMessageBox::information(this, tr("压力读数"),
        tr("当前压力为 %1 MPa").arg(pressure, 0, 'f', 1));*/
        // TODO: Add your control notification handler code here
   if (m_bCheckTimer)
        PacketType = 0x04;
    else
        Rdpressworkpara();
  // MyDialog_press dialog(this);
   /*ui->pidParaPEdit->setText(QString::number(m_fltEditParaP, 'f', 2));
   ui->pidParaIEdit->setText(QString::number(m_fltEditParaI, 'f', 2));
   ui->pidParaDEdit->setText(QString::number(m_fltEditParaD, 'f', 2))*/
   /* ui->pressureAlmHEdit->setText(QString::number(m_fltEditPressAlmH, 'f', 2));
            ui->pressureAlmLEdit->setText(QString::number(m_fltEditPressAlmL, 'f', 2));
            ui->pidParaPEdit->setText(QString::number(m_fltEditParaP, 'f', 2));
            ui->pidParaIEdit->setText(QString::number(m_fltEditParaI, 'f', 2));
            ui->pidParaDEdit->setText(QString::number(m_fltEditParaD, 'f', 2));*/
 
   /*dialog.m_nameEdit->setText(QString::number(m_fltEditPressAlmH, 'f', 2));
   dialog.m_nameEdit2->setText(QString::number(m_fltEditPressAlmL, 'f', 2));
   dialog.m_nameEditP->setText(QString::number(m_fltEditParaP, 'f', 2));
   dialog.m_nameEditI->setText(QString::number(m_fltEditParaI, 'f', 2));
   dialog.m_nameEditD->setText(QString::number(m_fltEditParaD, 'f', 2));*/




  // if (dialog.exec() == QDialog::Accepted) {
       // 可选：获取对话框数据
       // 这里不做额外处理，仅演示对话框的显示
   /*    dialog.m_nameEdit->setText(QString::number(m_fltEditPressAlmH, 'f', 2));
       dialog.m_nameEdit2->setText(QString::number(m_fltEditPressAlmL, 'f', 2));
       dialog.m_nameEditP->setText(QString::number(m_fltEditParaP, 'f', 2));
       dialog.m_nameEditI->setText(QString::number(m_fltEditParaI, 'f', 2));
       dialog.m_nameEditD->setText(QString::number(m_fltEditParaD, 'f', 2));*/
  // }


}



void MainWindow::Rdpressworkpara()
{
    qDebug()<<"enter into  Rdpressworkpara";
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
    qDebug()<<"Rdpressworkpara   00000";

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
        qDebug()<<"Rdpressworkpara   发送成功";

	}
    qDebug()<<"Rdpressworkpara   11111";

	Sleep(10);		//等待10毫秒，经过验证，延时1毫秒也工作正常

	i = 0;
	flag = 0;
	do {
		//BUF_SZIE是最大接收包长度，如果小于过来的数据包长度，会出错
		//recvfrom好像是个阻塞的函数
		i++;
		r_len = recvfrom(sss, buf, BUF_SZIE, 0, (SOCKADDR*)&clientAddr, &nClientLen);
        qDebug()<<"Rdpressworkpara  r_len:"<<r_len;
        qDebug()<<"Rdpressworkpara  SOCKET_ERROR:"<<SOCKET_ERROR;
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
    } while ((i <= 10000) && (flag == 0)); //收到的数据10000也没有错误，就是flag也没有变成1

    qDebug()<<"Rdpressworkpara   22222, i is： AND flag is: " <<i << flag;

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
            //ui->speedValueLabel_2->setText(QString("%1 MPa").arg(m_fltEditPressAlmH));
            //speedValueLabel_2
			m_fltEditPressAlmL = m_fPressAlmL;
            //ui->speedValueLabel_3->setText(QString("%1 MPa").arg(m_fltEditPressAlmL));

			m_fltEditParaP = m_fPidParaP;
			m_fltEditParaI = m_fPidParaI;
			m_fltEditParaD = m_fPidParaD;
			//UpdateData(FALSE);
            //使用qt的setText() 给控件赋值
           /* ui->pressureAlmHEdit->setText(QString::number(m_fltEditPressAlmH, 'f', 2));
            ui->pressureAlmLEdit->setText(QString::number(m_fltEditPressAlmL, 'f', 2));
            ui->pidParaPEdit->setText(QString::number(m_fltEditParaP, 'f', 2));
            ui->pidParaIEdit->setText(QString::number(m_fltEditParaI, 'f', 2));
			ui->pidParaDEdit->setText(QString::number(m_fltEditParaD, 'f', 2));*/
		}
		return;	//正确
	}
    else //收不到下位机的数据
	{
        //	CString str;
        //	str.Format("下位机回应数据包超时！");
        //	AfxMessageBox(str.GetBuffer(str.GetLength()));
        //	m_intEditErr++;		//统计
		//UpdateData(FALSE);
        return;	//错误
	}
}
void MainWindow::on_StopLoopoilButton_clicked()
{
    //if (ui->StopLoopoil->text() == "停止补油")
    //{
    //    ui->StopLoopoil->setText("启动补油");
    //    // 停止补油相关操作
    //}
    //else
    //{
    //    ui->StopLoopoil->setText("停止补油");
    //    // 启动补油相关操作
    //}
    // TODO: Add your control notification handler code here
    switch (WorkCmd)
    {
    case 0:		//当前待机状态
    case 1:		//当前工作状态
        WorkCmd = 2;		//进入泄压状态
        break;

    case 2:		//当前泄压状态
        WorkCmd = 0;		//进入待机状态
        break;
    }

    if (m_bCheckTimer)
        PacketType = 0x05;
    else
        SetPressWorkStatus();
}
void MainWindow::on_start_stopButton_clicked()
{
 //   if (ui->start_stop->text() == "启动")
 //   {
 //       ui->start_stop->setText("停止");
 //       // 启动相关操作
 //   }
 //   else
 //   {
 //       ui->start_stop->setText("启动");
 //       // 停止相关操作
	//}

    switch (WorkCmd)
    {
    case 0:		//当前待机状态
        WorkCmd = 1;		//进入工作状态
        break;

    case 1:		//当前工作状态
    case 2:		//当前泄压状态
        WorkCmd = 0;		//进入待机状态
        break;
    }

    if (m_bCheckTimer)
        PacketType = 0x05;
    else
        SetPressWorkStatus();
}

void MainWindow::on_setPressureButton_clicked()
{
    /*
    bool ok;
    double newPressure = QInputDialog::getDouble(this, tr("设置压力"),
        tr("请输入压力值 (0-40 MPa):"),
        ui->gaugeSpeed->value(),
        0, 40, 1, &ok);
    if (ok) {
        ui->gaugeSpeed->setValue(newPressure);
        ui->speedValueLabel->setText(QString("%1 MPa").arg(newPressure, 0, 'f', 1));

        if (newPressure > 85) {
            ui->statusLabel->setText("超压警告！");
            ui->statusLabel->setStyleSheet("color: red; font-weight: bold;");
        }
        else if (newPressure > 70) {
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
    */
    
    if (m_bCheckTimer)
        PacketType = 0x03;
    else
        Wrpressworkpara();

   // ui->gaugeSpeed->setValue(6);// = m_fPressureSet;
	//MyDialog_press dialog1(this);
	MyDialog dialog(this);

    //dialog.m_nameEdit->setText(QString::number(m_fltEditPressAlmH, 'f', 2));
    //dialog.m_nameEdit2->setText(QString::number(m_fltEditPressAlmL, 'f', 2));
    //dialog.m_nameEditP->setText(QString::number(m_fltEditParaP, 'f', 2));
    //dialog.m_nameEditI->setText(QString::number(m_fltEditParaI, 'f', 2));
    //dialog.m_nameEditD->setText(QString::number(m_fltEditParaD, 'f', 2));

    if (dialog1.exec() == QDialog::Accepted) {
    /*    dialog.m_nameEdit->setText(QString::number(m_fltEditPressAlmH, 'f', 2));
        dialog.m_nameEdit2->setText(QString::number(m_fltEditPressAlmL, 'f', 2));
        dialog.m_nameEditP->setText(QString::number(m_fltEditParaP, 'f', 2));
        dialog.m_nameEditI->setText(QString::number(m_fltEditParaI, 'f', 2));
        dialog.m_nameEditD->setText(QString::number(m_fltEditParaD, 'f', 2));*/
    }
    //m_fPressureSet = dialog.m_pressSetEdit->text().toFloat();
	m_fltEditPressureCurr = m_fPressureSet;
    ui->gaugeSpeed->setValue(m_fltEditPressureCurr);

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
   /* double level = ui->gaugeRpm->value();
    QMessageBox::information(this, tr("液位读数"),
        tr("当前液位为 %1 MPa").arg(level, 0, 'f', 1));*/
    // TODO: Add your control notification handler code here
   

    //if (m_bCheckTimer)
    //    PacketType = 0x02;
    //else
        Rdywpara();

    ui->gaugeRpm->m_warningLow = m_fYwAlmL;  // 0.2 默认警告低限 20
    ui->gaugeRpm->m_alarmHigh = m_fYwAlmH;// (1.8)    // 默认警报高限 80

    MyDialog dialog(this);
    dialog.m_nameEdit->setText(QString::number(m_fYwAlmH));// .toFloat();
    dialog.m_nameEdit2->setText(QString::number(m_fYwAlmL)); // .toFloat();

    ////m_fYwWorkH = dialog.m_nameEdit3->text().toFloat();
    dialog.m_nameEdit3->setText(QString::number(m_fYwWorkH));// .toFloat();
    ////m_fYwWorkL = dialog.m_nameEdit4->text().toFloat();
    dialog.m_nameEdit4->setText(QString::number(m_fYwWorkL));// .toFloat();

    if (dialog.exec() == QDialog::Accepted) {
        // 可选：获取对话框数据
        // 这里不做额外处理，仅演示对话框的显示
        
    }

    


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

 /*   m_fYwAlmH = m_fltEditYwAlmH;
    m_fYwAlmL = m_fltEditYwAlmL;
    m_fYwWorkH = m_fltEditYwWorkH;
    m_fYwWorkL = m_fltEditYwWorkL;*/

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
    int result = sendto(sss, t_buf, 23, 0, (SOCKADDR*)&CservAddr, nServAddlen);
    if (result == SOCKET_ERROR)
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
           // ui->rpmValueLabel_2->setText(QString("%1 MPa").arg(m_fYwAlmH));
            //根据此数值，确定表的指针摆幅范围。
            //仪表盘的告警高限


            Fconverter.b[3] = r_buf[9];		//32位数据，四个字节浮点数
            Fconverter.b[2] = r_buf[10];	//32位数据，四个字节浮点数
            Fconverter.b[1] = r_buf[11];	//32位数据，四个字节浮点数
            Fconverter.b[0] = r_buf[12];	//32位数据，四个字节浮点数
            m_fYwAlmL = Fconverter.f;
           // ui->rpmValueLabel_3->setText(QString("%1 MPa").arg(m_fYwAlmL));


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
          //  ui->rpmValueLabel_2->setText(QString("%1 MPa").arg(m_fltEditYwWorkL));
            m_fltEditYwAlmL = m_fYwAlmL;
            m_fltEditYwWorkH = m_fYwWorkH;
            m_fltEditYwWorkL = m_fYwWorkL;
//            ui->rpmValueLabel->setText(QString("%1 MPa").arg(m_fltEditYwWorkL));


            if (r_buf[21])
            {
                m_bCheckAutoOilEnable = 1;                
               // ui->checkBox->setChecked(true);
            }
            else
            {
                m_bCheckAutoOilEnable = 0;
                //ui->checkBox->setChecked(true);
            }

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
   /* bool ok;
    double newLevel = QInputDialog::getDouble(this, tr("设置液位"),
        tr("请输入液位值 (0-100 MPa):"),
        ui->gaugeRpm->value(),
        0, 100, 1, &ok);
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
    }*/
    MyDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // 可选：获取对话框数据
        // 这里不做额外处理，仅演示对话框的显示
    }
	//qDebug() << dialog.m_nameLabel->text().toFloat();
    m_fYwAlmH = dialog.m_nameEdit->text().toFloat();
    m_fYwAlmL = dialog.m_nameEdit2->text().toFloat();

    m_fYwWorkH = dialog.m_nameEdit3->text().toFloat();
    m_fYwWorkL = dialog.m_nameEdit4->text().toFloat();







    // TODO: Add your control notification handler code here
    if (m_bCheckTimer)
        PacketType = 0x01;
    else
        Wrywpara();

}