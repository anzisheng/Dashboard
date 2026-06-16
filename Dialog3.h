#ifndef DIALOG3_H
#define DIALOG3_H

#include <QDialog>
#include <QCheckBox>

class QLineEdit;
class QPushButton;
class QCheckBox;

class Dialog3 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog3(QWidget* parent = nullptr);
    ~Dialog3() override = default;

private slots:
    void onOkClicked();

public:
    QCheckBox* m_bCheckStopIn = nullptr;//¼±Í£
    QCheckBox* m_bCheckOverPressure = nullptr;//¹ýÑ¹±¨¾¯
    QCheckBox* m_bCheckRelief = nullptr;//Ð¹Ñ¹·§
    QCheckBox* m_bCheckBeep = nullptr;//·äÃùÆ÷
        
    QCheckBox* m_bCheckPump = nullptr;//²¹ÓÍ±Ã
   

    QLineEdit* m_lineEdit = nullptr;
    QPushButton* m_okButton = nullptr;
};

#endif // DIALOG3_H