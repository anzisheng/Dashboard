#include "Dialog2.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEvent>
#include <QMouseEvent>

Dialog2::Dialog2(QWidget* parent)
    : QDialog(parent), m_dialog(nullptr)
{
    setWindowTitle(QStringLiteral("对话框 2"));
    setMinimumSize(300, 250);

    auto* layout = new QVBoxLayout(this);

    auto* label = new QLabel(QStringLiteral("请输入液位参数"), this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    m_fltEditYwAlmH = new QLineEdit(this);
    m_fltEditYwAlmH->setPlaceholderText(QStringLiteral("液位高限报警阈值"));
    layout->addWidget(m_fltEditYwAlmH);
    m_fltEditYwAlmH->installEventFilter(this);

    m_fltEditYwAlmL = new QLineEdit(this);
    m_fltEditYwAlmL->setPlaceholderText(QStringLiteral("液位低限报警阈值"));
    layout->addWidget(m_fltEditYwAlmL);
    m_fltEditYwAlmL->installEventFilter(this);
    

    m_fltEditYwWorkH = new QLineEdit(this);
    m_fltEditYwWorkH->setPlaceholderText(QStringLiteral("液位高限停止值"));
    layout->addWidget(m_fltEditYwWorkH);
    m_fltEditYwWorkH->installEventFilter(this);
    m_fltEditYwWorkL = new QLineEdit(this);
    m_fltEditYwWorkL->setPlaceholderText(QStringLiteral("液位低限启动值"));
    layout->addWidget(m_fltEditYwWorkL);

    m_okButton = new QPushButton(QStringLiteral("确定"), this);
    layout->addWidget(m_okButton);

    connect(m_okButton, &QPushButton::clicked, this, &Dialog2::onOkClicked);
}

void Dialog2::onOkClicked()
{
    //qDebug() << "对话框2 输入内容:" << m_lineEdit->text();
    accept();
}
bool Dialog2::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        // 判断点击的是否是我们关心的任意一个编辑框
        if (obj == m_fltEditYwWorkH ||
            obj == m_fltEditYwWorkL ||
            obj == m_fltEditYwAlmL) {
            m_currentEdit = qobject_cast<QLineEdit*>(obj);
            onLineEditClicked();
            return true;   // 拦截事件，阻止焦点移动
        }
    }
    return QDialog::eventFilter(obj, event);
}
#include "NumpadDialog.h"

void Dialog2::onLineEditClicked()
{
    if (!m_currentEdit) return;

    // 延迟创建数字键盘对话框
    if (!m_dialog) {
        m_dialog = new NumPadDialog(this);
        connect(m_dialog, &QDialog::accepted, this, &Dialog2::onDialogAccepted);
    }

    // 将当前编辑框的文本设置为对话框的初始值
    m_dialog->setText(m_currentEdit->text());

    // 模态显示
    m_dialog->exec();
}

void Dialog2::onDialogAccepted()
{
    if (!m_currentEdit) return;

    // 从对话框获取输入，回填到当前编辑框
    QString newText = m_dialog->getText();
    m_currentEdit->setText(newText);
}