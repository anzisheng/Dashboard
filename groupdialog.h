#ifndef GROUPDIALOG_H
#define GROUPDIALOG_H

#include <QDialog>
#include <QString>

class QLabel;
class QLineEdit;
class QGroupBox;
class QCheckBox;
class QDialogButtonBox;
class QGridLayout;
class QVBoxLayout;

class GroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupDialog(QWidget *parent = nullptr);
    ~GroupDialog();

    // 获取输入数据
    QString getField1() const;
    QString getField2() const;
    QString getField3() const;
    QString getField4() const;
    bool getCheckBoxState() const;

    // 设置默认值
    void setField1(const QString &value);
    void setField2(const QString &value);
    void setField3(const QString &value);
    void setField4(const QString &value);
    void setCheckBoxState(bool checked);

    // 清除所有输入
    void clearInputs();

private slots:
    void onInputChanged();

private:
    void setupUI();
    void setupConnections();

    // UI组件
    QGroupBox *m_groupBox;
    QLabel *m_label1;
    QLabel *m_label2;
    QLabel *m_label3;
    QLabel *m_label4;
    QLineEdit *m_lineEdit1;
    QLineEdit *m_lineEdit2;
    QLineEdit *m_lineEdit3;
    QLineEdit *m_lineEdit4;
    QCheckBox *m_checkBox;
    QDialogButtonBox *m_buttonBox;

    // 布局
    QVBoxLayout *m_mainLayout;
    QGridLayout *m_gridLayout;
};

#endif // GROUPDIALOG_H