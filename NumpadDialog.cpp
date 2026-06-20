#include "numpaddialog.h"
#include "numpad.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

NumPadDialog::NumPadDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    // 当数字键盘的 enter 被点击时，相当于确认
    connect(m_numPad, &NumPad::enterClicked, this, &NumPadDialog::onEnterClicked);
}

NumPadDialog::~NumPadDialog() {}

void NumPadDialog::setupUI()
{
    setWindowTitle("数字键盘");
    setModal(true);   // 模态对话框

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 显示输入的区域
    m_display = new QLineEdit(this);
    m_display->setPlaceholderText("请输入数字");
    m_display->setAlignment(Qt::AlignRight);
    m_display->setReadOnly(true);   // 禁止直接键盘输入，只能通过数字键盘
    mainLayout->addWidget(m_display);

    // 数字键盘
    m_numPad = new NumPad(this);
    m_numPad->setDisplay(m_display);   // 将键盘与显示框关联
    mainLayout->addWidget(m_numPad);

    // 确认/取消按钮
    QHBoxLayout *btnLayout = new QHBoxLayout;
    QPushButton *okBtn = new QPushButton("确认", this);
    QPushButton *cancelBtn = new QPushButton("取消", this);
    btnLayout->addStretch();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
    mainLayout->addLayout(btnLayout);

    // 连接按钮信号
    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void NumPadDialog::setText(const QString &text)
{
    m_display->setText(text);
}

QString NumPadDialog::getText() const
{
    return m_display->text();
}

void NumPadDialog::onEnterClicked()
{
    // 当数字键盘的确认键被点击时，相当于点击对话框的"确认"按钮
    accept();
}

void NumPadDialog::onCancelClicked()
{
    reject();
}