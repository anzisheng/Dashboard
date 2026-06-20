#ifndef NUMPADDIALOG_H
#define NUMPADDIALOG_H

#include <QDialog>
#include <QLineEdit>

class NumPad;

class NumPadDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NumPadDialog(QWidget *parent = nullptr);
    ~NumPadDialog();

    // 设置/获取对话框中的文本
    void setText(const QString &text);
    QString getText() const;

private slots:
    void onEnterClicked();    // 数字键盘的确认键触发对话框确认
    void onCancelClicked();   // 取消按钮

private:
    void setupUI();

    QLineEdit *m_display;
    NumPad    *m_numPad;
};

#endif // NUMPADDIALOG_H