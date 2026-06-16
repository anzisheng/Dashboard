#include "Dialog3.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

Dialog3::Dialog3(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(QStringLiteral("对话框 1"));
    setMinimumSize(300, 150);

    auto* layout = new QVBoxLayout(this);

    auto* label = new QLabel(QStringLiteral("这是第一个对话框"), this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setPlaceholderText(QStringLiteral("请输入内容..."));
    layout->addWidget(m_lineEdit);

    m_okButton = new QPushButton(QStringLiteral("确定"), this);
    layout->addWidget(m_okButton);

    connect(m_okButton, &QPushButton::clicked, this, &Dialog3::onOkClicked);
}

void Dialog3::onOkClicked()
{
    qDebug() << "对话框1 输入内容:" << m_lineEdit->text();
    accept();
}