#ifndef MYDIALOG_Press_H
#define MYDIALOG_Press_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>

class MyDialog_press : public QDialog
{
    Q_OBJECT

public:
    explicit MyDialog_press(QWidget *parent = nullptr);
    ~MyDialog_press();

private slots:
    void onOkClicked();
    void onCancelClicked();

private:
    void setupUI();
public:

    // 控件成员
    QLabel *m_nameLabel;
    QLabel *m_nameLabel2;
    QLineEdit *m_nameEdit;
    QLineEdit *m_nameEdit2;

    QLabel* PID_P;
    QLabel* PID_I;
    QLabel* PID_D;
    QLineEdit* m_nameEditP;
    QLineEdit* m_nameEditI;
    QLineEdit* m_nameEditD;

    QLabel *m_ageLabel;
    QSpinBox *m_ageSpinBox;
    
    QLabel *m_genderLabel;
    QComboBox *m_genderCombo;
    
    QCheckBox *m_subscribeCheckBox;
    
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
};

#endif // MYDIALOG_H