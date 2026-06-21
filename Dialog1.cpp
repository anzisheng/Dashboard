#include "Dialog1.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include "NumpadDialog.h"

Dialog1::Dialog1(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(QStringLiteral("对话框 1"));
    setMinimumSize(300, 250);

    auto* layout = new QVBoxLayout(this);

    auto* label = new QLabel(QStringLiteral("设置压力工作参数"), this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    m_fltEditPressAlmH = new QLineEdit(this);
    m_fltEditPressAlmH->setPlaceholderText(QStringLiteral("压力告警高限"));
    layout->addWidget(m_fltEditPressAlmH);
    m_fltEditPressAlmH->installEventFilter(this);

    m_fltEditPressAlmL = new QLineEdit(this);
    m_fltEditPressAlmL->setPlaceholderText(QStringLiteral("压力告警低限"));
    layout->addWidget(m_fltEditPressAlmL);
    
    m_fltEditParaP = new QLineEdit(this);
    m_fltEditParaP->setPlaceholderText(QStringLiteral("P参数"));
    layout->addWidget(m_fltEditParaP);

    m_fltEditParaI = new QLineEdit(this);
    m_fltEditParaI->setPlaceholderText(QStringLiteral("I参数"));
    layout->addWidget(m_fltEditParaI);
    m_fltEditParaD = new QLineEdit(this);
    m_fltEditParaD->setPlaceholderText(QStringLiteral("D参数"));
    layout->addWidget(m_fltEditParaD);
    m_okButton = new QPushButton(QStringLiteral("确定"), this);
    layout->addWidget(m_okButton);

    connect(m_okButton, &QPushButton::clicked, this, &Dialog1::onOkClicked);
}
bool Dialog1::eventFilter(QObject* obj, QEvent* event)
{
    // 捕获目标编辑框的鼠标点击事件
    if (obj == m_fltEditPressAlmH && event->type() == QEvent::MouseButtonPress) {
        onLineEditClicked();
        return true;   // 事件已处理，不再传递（避免焦点变化）
    }
    return QDialog::eventFilter(obj, event);
}

void Dialog1::onLineEditClicked()
{
    // 延迟创建对话框（只创建一次）
    if (!m_dialog) {
        m_dialog = new NumPadDialog(this);
        connect(m_dialog, &QDialog::accepted, this, &Dialog1::onDialogAccepted);
    }

    // 将当前编辑框的内容作为初始文本
    m_dialog->setText(m_fltEditPressAlmH->text());

    // 模态弹出数字键盘
    m_dialog->exec();
}

void Dialog1::onDialogAccepted()
{
    // 从对话框获取输入结果，回填到编辑框
    QString newText = m_dialog->getText();
    m_fltEditPressAlmH->setText(newText);
}

void Dialog1::onOkClicked()
{
   // qDebug() << "对话框1 输入内容:" << m_lineEdit->text();

    accept();
}