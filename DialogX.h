#ifndef DIALOGLX_H
#define DIALOGLX_H

#include <QDialog>
#include <QLabel>
#include <QGraphicsView>
#include <QDialog>
#include <QString>
#include <QPixmap>
class QGroupBox;
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
class QCheckBox;

class QGraphicsView;
class QLineEdit;
class QPushButton;
class NumPadDialog;
#include "mainwindow.h"

class DialogX : public QDialog
{
    Q_OBJECT

public:
    explicit DialogX(QWidget* parent = nullptr);
    ~DialogX() override = default;
	MainWindow* m_parent = nullptr; // 指向父窗口的指针
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;   // ① 重写事件过滤器
private slots:
    void onOkClicked();
private slots:
    void onLineEditClicked();    // ② 点击编辑框时调用的槽
    void onDialogAccepted();     // ③ 数字键盘确认时调用的槽

private:
    void setupImageScene();
    public:
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
    QPushButton* m_okButton = nullptr;
    NumPadDialog* m_dialog = nullptr; // 数字键盘对话框指针
    QLineEdit* m_currentEdit = nullptr;
    QLineEdit* m_nameEdit;
    QLineEdit* m_emailEdit;

    QGraphicsView *graphicsView;

    // UI组件 - 图像显示区域（顶部）
    QGroupBox* m_imageGroupBox;
    QGraphicsView* m_graphicsView;
    //QGraphicsScene* m_graphicsScene;
    QGraphicsScene* m_graphicsScene;
    QPushButton* m_loadImageBtn;
    QPushButton* m_clearImageBtn;
    QPushButton* m_fitImageBtn;
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
    QCheckBox* m_checkBox;
    QDialogButtonBox* m_buttonBox;

    // UI组件 - 地址信息区域（右下，3对）
    QGroupBox* m_addressGroupBox;
    QLabel* m_labelAddress;
    QLabel* m_labelCity;
    QLabel* m_labelZipCode;
    QLineEdit* m_lineEditAddress;
    QLineEdit* m_lineEditCity;
    QLineEdit* m_lineEditZipCode;

    //QDialogButtonBox* m_buttonBox;
    QPushButton* m_writeButton;

    // 布局
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_bottomLayout;
    QVBoxLayout* m_imageLayout;
    QHBoxLayout* m_imageButtonLayout;
    QGridLayout* m_personalInfoLayout;
    QGridLayout* m_addressLayout;
public:
    void onRdButtonClicked();//读取
    void onWriteButtonClicked();

};


#endif // DIALOG1_H