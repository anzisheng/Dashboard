#ifndef DIALOG3_H
#define DIALOG3_H

#include <QDialog>
#include <QCheckBox>

class QLineEdit;
class QPushButton;
class QCheckBox;
#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>

class Dialog3 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog3(QWidget* parent = nullptr);
    ~Dialog3() override = default;

private slots:
    //void onOkClicked();

public:
    void setupUI();

    // UI×é¼þ
    QGroupBox* dmxGroupBox;
    QLabel* addressLabel;
    QLineEdit* addressLineEdit;
    QLabel* ch1Label;
    QLabel* ch2Label;
    QLabel* ch3Label;
    QLabel* ch4Label;
    QLineEdit* ch1LineEdit;
    QLineEdit* ch2LineEdit;
    QLineEdit* ch3LineEdit;
    QLineEdit* ch4LineEdit;
    QPushButton* exitButton;

    //QCheckBox* m_bCheckStopIn = nullptr;//¼±Í£
    //QCheckBox* m_bCheckOverPressure = nullptr;//¹ýÑ¹±¨¾¯
    //QCheckBox* m_bCheckRelief = nullptr;//Ð¹Ñ¹·§
    //QCheckBox* m_bCheckBeep = nullptr;//·äÃùÆ÷
    //    
    //QCheckBox* m_bCheckPump = nullptr;//²¹ÓÍ±Ã
   

    QLineEdit* m_lineEdit = nullptr;
    QPushButton* m_okButton = nullptr;
};

#endif // DIALOG3_H