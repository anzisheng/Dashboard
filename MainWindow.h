#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <atlstr.h>
#include "MyDialog.h"
#include "MyDialog_press.h"
#include "numpaddialog.h"
class NumPadDialog;

//enum class  Precision_an {
//    PRECISION_0 = 0,
//    PRECISION_1 = 1,
//    PRECISION_2 = 2,
//    PRECISION_3 = 3
//};
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
    //void Rdpressworkpara();
    bool InitInstance();
    unsigned char RandomCount;
    unsigned char RandomNum;
    
    MyDialog dialog;// (this);
    MyDialog_press dialog1;// (this);

    void ReadData();

    union {
        float f;
        unsigned char b[4];
    } Fconverter;

    union {
        unsigned int i;
        unsigned char b[4];
    } Iconverter;
    float m_fYwAlmH;		//液位高限报警阈值
    float m_fYwAlmL;		//液位低限报警阈值
    float m_fYwWorkH;		//液位高限停止值
    float m_fYwWorkL;		//液位低限启动值


    float m_fPressureSet;	//压力设定值

    float m_fPressAlmH;		//压力高限报警阈值
    float m_fPressAlmL;		//压力低限报警阈值
    float m_fPidParaP;
    float m_fPidParaI;
    float m_fPidParaD;

    unsigned char WorkCmd;	//设定的工作状态值

    unsigned char PacketType;

    void Wrywpara();		//写液位设置参数
    void Rdywpara();		//读液位设置参数

    void Wrpressworkpara();
    void Rdpressworkpara();
    void SetPressWorkStatus();	//设置压力部分工作状态

/// <summary>
   // enum { IDD = IDD_EFH1_DIALOG };
    bool m_bCheckAutoOilEnable;
    bool m_bCheckBeep;
    bool m_bCheckOverPressure;
    bool m_bCheckPump;
    bool m_bCheckRelief;
    bool m_bCheckStopIn;
    bool m_bCheckTimer;
    float	m_fltEditBpqFffCurr;
    float	m_fltEditBpqFff;
    int		m_intEditDmxIn0;
    int		m_intEditDmxIn1;
    int		m_intEditDmxIn2;
    int		m_intEditDmxIn3;
    float	m_fltEditPressureCurr;
    float	m_fltEditPressureSet;
    QVector<float> m_vecPressures; 
    //float   m_fPressureSet;	//压力设定值
    CString	m_strEditStatus;
    float	m_fltEditYwAlmH;
    float	m_fltEditYwAlmL;
    float	m_fltEditYwCurr;
    float	m_fltEditYwWorkH;
    float	m_fltEditYwWorkL;
    CString	m_strEditYwStatus;
    float	m_fltEditPressAlmH;
    float	m_fltEditPressAlmL;
    float	m_fltEditParaD;
    float	m_fltEditParaI;
    float	m_fltEditParaP;
    CString	m_strEditPressureAlarmStatus;
    int		m_intRadioWorkStatus;
    /// </summary>
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
private slots:
    void onLineEditClicked();          // 处理编辑框点击事件
    void onDialogAccepted();           // 对话框确认时更新输入框
    //void onDialogAccepted();           // 对话框确认时更新输入框
    void on_updateButton_clicked();
    void updateInfo();
    void on_readPressureButton_clicked();
    void on_setPressureButton_clicked();
    void on_start_stopButton_clicked();
    void on_StopLoopoilButton_clicked();
    void on_readLevelButton_clicked();
    void on_setLevelButton_clicked();
    void on_pressUpdateButton_clicked();
    
	void on_pressParaButton_2_clicked();//pushButton_2
    void on_ywParaButton_5_clicked();//pushButton_5
    
    void on_MMXParaButton_4_clicked();//pushButton_4
    //void onCheckBoxToggled(bool checked);
    void updateTime(); // 定时器要执行的槽函数

private:
    Ui::MainWindow* ui;
    QPushButton* m_openButton;
    QTimer* m_timer;
    NumPadDialog* m_dialog = nullptr;  // 数字键盘对话框
    //unsigned char PacketType;
};

#endif