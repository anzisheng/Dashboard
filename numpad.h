#ifndef NUMPAD_H
#define NUMPAD_H

#include <QWidget>
#include <QLineEdit>

class NumPad : public QWidget
{
    Q_OBJECT
public:
    explicit NumPad(QWidget *parent = nullptr);
    void setDisplay(QLineEdit *display);

signals:
    void digitClicked(const QString &digit);
    void backspaceClicked();
    void clearClicked();
    void enterClicked();   // 确认键信号

private slots:
    void onDigitButtonClicked();
    void onBackspaceClicked();
    void onClearClicked();
    void onEnterClicked();

private:
    void setupUI();
    QLineEdit *m_display;
};

#endif // NUMPAD_H