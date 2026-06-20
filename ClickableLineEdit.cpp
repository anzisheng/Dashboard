#include "ClickableLineEdit.h"

ClickableLineEdit::ClickableLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    setReadOnly(true);   // 只读，禁止软键盘弹出
}

void ClickableLineEdit::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
    // 注意：不调用基类，以阻止默认的焦点行为（可选）
    // 如果希望保留默认行为，可以调用 QLineEdit::mousePressEvent(event);
    // 但此处我们只发射信号，不调用基类以避免触发输入法。
}