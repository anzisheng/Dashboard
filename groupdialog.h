#ifndef GROUPDIALOG_H
#define GROUPDIALOG_H

#include <QDialog>
#include <QString>
#include <QPixmap>
#include <QVector>
#include <QPointF>
#include <QTimer>
#include <QSettings>
class QLabel;
class QLineEdit;
class QGroupBox;
class QDialogButtonBox;
class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QGraphicsView;
class QGraphicsScene;
class QPushButton;
class NumPadDialog;
#include "mainwindow.h"

class GroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupDialog(QWidget* parent = nullptr);
    ~GroupDialog();
	MainWindow* m_parent = nullptr; // 指向父窗口的指针
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;   // ① 重写事件过滤器
protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onLineEditClicked();    // ② 点击编辑框时调用的槽
    void onDialogAccepted();     // ③ 数字键盘确认时调用的槽

    // 获取用户输入的数据 - 个人信息（5对）
    QString getName() const;
    QString getEmail() const;
    QString getPhone() const;
    QString getBirthday() const;
    QString getOccupation() const;

    // 获取用户输入的数据 - 地址信息（3对）
    QString getAddress() const;
    QString getCity() const;
    QString getZipCode() const;

    // 设置默认值 - 个人信息
    void setName(const QString& name);
    void setEmail(const QString& email);
    void setPhone(const QString& phone);
    void setBirthday(const QString& birthday);
    void setOccupation(const QString& occupation);

    // 设置默认值 - 地址信息
    void setAddress(const QString& address);
    void setCity(const QString& city);
    void setZipCode(const QString& zipCode);

    // 清除所有输入
    void clearInputs();

    // 曲线绘制方法
    void drawCoordinateSystem();
    void drawCurves();
    void clearCurves();
    void addRandomDataPoint();

private slots:
    void onInputChanged();
    void onStartStopClicked();
    void onTimerTimeout();
    void onWriteButtonClicked();
	void onRdButtonClicked();//读取

//private slots:
//    //void onLineEditClicked();    // ② 点击编辑框时调用的槽
//    void onDialogAccepted();     // ③ 数字键盘确认时调用的槽

public:
    NumPadDialog* m_dialog = nullptr; // 数字键盘对话框指针
    void Rdpressworkpara();

private:
    void setupUI();
    void setupConnections();
    void setupImageScene();
    void generateInitialData();

    // UI组件 - 图像显示区域（顶部）
    QGroupBox* m_imageGroupBox;
    QGraphicsView* m_graphicsView;
    QGraphicsScene* m_graphicsScene;
    QPushButton* m_startStopBtn;
    QPixmap m_currentPixmap;


    QLineEdit* m_currentEdit = nullptr;
    
    void loadSettings();
    void saveSettings();

    QVector<QLineEdit*> m_edits;  // 
    static const int EDIT_COUNT = 8;// 5;

    // UI组件 - 个人信息区域（左下，5对）
    QGroupBox* m_personalInfoGroupBox;
    QLabel* m_labelName;
    QLabel* m_labelEmail;
    QLabel* m_labelPhone;
    QLabel* m_labelBirthday;
    QLabel* m_labelOccupation;
    QLineEdit* m_lineEditName;
    QLineEdit* m_lineEditEmail;
    QLineEdit* m_lineEditPhone;
    QLineEdit* m_lineEditBirthday;
    QLineEdit* m_lineEditOccupation;

    QLineEdit* m_fltEditPressAlmH = nullptr;
    QLabel* m_fltLabelPressAlmH = nullptr;
    QLineEdit* m_fltEditPressAlmL = nullptr;
    QLabel* m_fltLabelPressAlmL = nullptr;
    QLineEdit* m_fltEditParaP = nullptr;
    QLabel* m_fltLabelParaP = nullptr;
    QLineEdit* m_fltEditParaI = nullptr;
    QLabel* m_fltQLabelParaI = nullptr;
    QLineEdit* m_fltEditParaD = nullptr;
    QLabel* m_fltLabelParaD = nullptr;
//    QLineEdit* m_currentEdit = nullptr;

    // UI组件 - 地址信息区域（右下，3对）
    QGroupBox* m_addressGroupBox;
    QLabel* m_labelAddress;
    QLabel* m_labelCity;
    QLabel* m_labelZipCode;
    QLineEdit* m_lineEditAddress;
    QLineEdit* m_lineEditCity;
    QLineEdit* m_lineEditZipCode;

    QDialogButtonBox* m_buttonBox;
    QPushButton* m_writeButton;

    // 布局
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_bottomLayout;
    QVBoxLayout* m_imageLayout;
    QHBoxLayout* m_imageButtonLayout;
    QGridLayout* m_personalInfoLayout;
    QGridLayout* m_addressLayout;

    // 曲线数据
    QVector<QPointF> m_constData;      // 恒定值数据 y=0.8
    QVector<QPointF> m_randomData;     // 随机值数据
    QTimer* m_timer;
    bool m_isRunning;
    int m_maxDataPoints;               // 最大数据点数
    double m_currentTime;              // 当前时间（秒）
    double m_displayDuration;          // 显示的时间范围（秒）
    bool m_coordinateSystemDrawn;      // 坐标系是否已绘制

    // 坐标系参数
    static constexpr int MARGIN = 50;
    static constexpr double Y_MIN = 0.0;
    static constexpr double Y_MAX = 2.0;
    static constexpr double CONST_VALUE = 0.8;
};

#endif // GROUPDIALOG_H