#include "Dialog3.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

Dialog3::Dialog3(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(QStringLiteral("DMX 输入"));
    setMinimumSize(300, 250);

    auto* layout = new QVBoxLayout(this);

    auto* label = new QLabel(QStringLiteral("请选择DMX输入"), this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    m_bCheckStopIn = new QCheckBox("急停",this);
    //m_lineEdit->setPlaceholderText(QStringLiteral("请输入内容..."));
    layout->addWidget(m_bCheckStopIn);

    m_bCheckOverPressure = new QCheckBox("过压报警", this);// nullptr;//过压报警
    layout->addWidget(m_bCheckOverPressure);

     m_bCheckRelief = new QCheckBox("泄压阀", this);// nullptr;//泄压阀
    layout->addWidget(m_bCheckRelief);

    m_bCheckBeep = new QCheckBox("蜂鸣器", this);// nullptr;//泄压阀
    layout->addWidget(m_bCheckBeep);

    m_bCheckPump = new QCheckBox("补油泵", this);// nullptr;//泄压阀
	layout->addWidget(m_bCheckPump);

    m_okButton = new QPushButton(QStringLiteral("确定"), this);
    layout->addWidget(m_okButton);

    connect(m_okButton, &QPushButton::clicked, this, &Dialog3::onOkClicked);
}

void Dialog3::onOkClicked()
{
    qDebug() << "对话框1 输入内容:" << m_lineEdit->text();
    accept();
}