#include "Dialog1.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

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

void Dialog1::onOkClicked()
{
   // qDebug() << "对话框1 输入内容:" << m_lineEdit->text();

    accept();
}