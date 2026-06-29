#ifndef GROUPDIALOG_H
#define GROUPDIALOG_H

#include <QDialog>
#include <QString>
#include <QPixmap>
#include <QVector>
#include <QPointF>
#include <QTimer>

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

class GroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupDialog(QWidget* parent = nullptr);
    ~GroupDialog();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;   // ① 重写事件过滤器

private slots:
    void onLineEditClicked();    // ② 点击编辑框时调用的槽
    void onDialogAccepted();     // ③ 数字键盘确认时调用的槽

public:
    NumPadDialog* m_dialog = nullptr; // 数字键盘对话框指针
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

    // 图像相关方法
    void setImage(const QPixmap& pixmap);
    void setImage(const QString& imagePath);
    void clearImage();

    // 曲线绘制方法
    void drawCoordinateSystem();
    void drawCurves();
    void addRandomDataPoint();

private slots:
    void onInputChanged();
    void onLoadImageClicked();
    void onClearImageClicked();
    void onStartStopClicked();
    void onTimerTimeout();

private:
    void setupUI();
    void setupConnections();
    void setupImageScene();
    void generateInitialData();

    // UI组件 - 图像显示区域（顶部）
    QGroupBox* m_imageGroupBox;
    QGraphicsView* m_graphicsView;
    QGraphicsScene* m_graphicsScene;
    QPushButton* m_loadImageBtn;
    QPushButton* m_clearImageBtn;
    QPushButton* m_startStopBtn;
    QPixmap m_currentPixmap;

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
    QLineEdit* m_currentEdit = nullptr;

    // UI组件 - 地址信息区域（右下，3对）
    QGroupBox* m_addressGroupBox;
    QLabel* m_labelAddress;
    QLabel* m_labelCity;
    QLabel* m_labelZipCode;
    QLineEdit* m_lineEditAddress;
    QLineEdit* m_lineEditCity;
    QLineEdit* m_lineEditZipCode;

    QDialogButtonBox* m_buttonBox;

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

    // 坐标系参数
    static constexpr int MARGIN = 50;
    static constexpr double Y_MIN = 0.0;
    static constexpr double Y_MAX = 2.0;
    /*static constexpr*/ double CONST_VALUE = 0.8;
};

#endif // GROUPDIALOG_H