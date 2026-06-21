#ifndef DIALOG1_H
#define DIALOG1_H

#include <QDialog>

class QLineEdit;
class QPushButton;
class NumPadDialog;

class Dialog1 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog1(QWidget* parent = nullptr);
    ~Dialog1() override = default;
protected:
    bool eventFilter(QObject* obj, QEvent* event) override;   // ① 重写事件过滤器
private slots:
    void onOkClicked();
private slots:
    void onLineEditClicked();    // ② 点击编辑框时调用的槽
    void onDialogAccepted();     // ③ 数字键盘确认时调用的槽

//private:
    public:
    QLineEdit* m_fltEditPressAlmH = nullptr;
    QLineEdit* m_fltEditPressAlmL = nullptr;
	QLineEdit* m_fltEditParaP = nullptr;
	QLineEdit* m_fltEditParaI = nullptr;
	QLineEdit* m_fltEditParaD = nullptr;
    QPushButton* m_okButton = nullptr;
    NumPadDialog* m_dialog = nullptr; // 数字键盘对话框指针
};

#endif // DIALOG1_H