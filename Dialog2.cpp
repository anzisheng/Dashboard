#include "Dialog2.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

Dialog2::Dialog2(QWidget* parent)
    : QDialog(parent)
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

    m_fltEditYwAlmL = new QLineEdit(this);
    m_fltEditYwAlmL->setPlaceholderText(QStringLiteral("液位低限报警阈值"));
    layout->addWidget(m_fltEditYwAlmL);

    m_fltEditYwWorkH = new QLineEdit(this);
    m_fltEditYwWorkH->setPlaceholderText(QStringLiteral("液位高限停止值"));
    layout->addWidget(m_fltEditYwWorkH);

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