#ifndef DIALOG2_H
#define DIALOG2_H

#include <QDialog>

class QLineEdit;
class QPushButton;
class NumPadDialog;
class Dialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog2(QWidget* parent = nullptr);
    ~Dialog2() override = default;
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
private slots:
    void onLineEditClicked();      // 点击任意编辑框时调用
    void onDialogAccepted();       // 数字键盘确认时回填


public:
    //QLineEdit* m_fltEditParaP;   // 目标编辑框（假设已定义）
    QLineEdit* m_fltEditYwAlmH = nullptr;
    QLineEdit* m_fltEditYwWorkH;   // 第一个编辑框
    QLineEdit* m_fltEditYwWorkL;   // 第二个编辑框
    QLineEdit* m_fltEditYwAlmL;    // 第三个编辑框（根据实际名称调整）

    QLineEdit* m_currentEdit;      // 记录当前被点击的编辑框
    NumPadDialog* m_dialog = nullptr;
private slots:
    void onOkClicked();

private:
public:

    QPushButton* m_okButton = nullptr;
};

#endif // DIALOG2_H