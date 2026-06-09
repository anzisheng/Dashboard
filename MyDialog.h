#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>

class MyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyDialog(QWidget *parent = nullptr);
    ~MyDialog();

private slots:
    void onOkClicked();
    void onCancelClicked();

private:
    void setupUI();

    // 控件成员
    QLabel *m_nameLabel;
    QLineEdit *m_nameEdit;
    
    QLabel *m_ageLabel;
    QSpinBox *m_ageSpinBox;
    
    QLabel *m_genderLabel;
    QComboBox *m_genderCombo;
    
    QCheckBox *m_subscribeCheckBox;
    
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
};

#endif // MYDIALOG_H