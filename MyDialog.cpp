#include "MyDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

MyDialog::MyDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUI();
    setWindowTitle("液位参数对话框");
    setModal(true);  // 设置为模态对话框
    resize(350, 250);
}

MyDialog::~MyDialog()
{
}

void MyDialog::setupUI()
{
    // 创建控件
    m_nameLabel = new QLabel("液位高限报警阈值:", this);
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("请输入高限报警阈值");

    m_nameLabel2 = new QLabel("液位低限报警阈值:", this);
    m_nameEdit2 = new QLineEdit(this);
    m_nameEdit2->setPlaceholderText("请输入低限报警阈值");

    m_nameLabel3 = new QLabel("液位高限停止值:", this);
    m_nameEdit3 = new QLineEdit(this);
    m_nameEdit3->setPlaceholderText("请输入液位高限停止值");

    m_nameLabel4 = new QLabel("液位低限启动值:", this);
    m_nameEdit4 = new QLineEdit(this);
    m_nameEdit4->setPlaceholderText("请输入液位低限启动值");

   /* m_ageLabel = new QLabel("年龄:", this);
    m_ageSpinBox = new QSpinBox(this);
    m_ageSpinBox->setRange(0, 150);
    m_ageSpinBox->setValue(18);

    m_genderLabel = new QLabel("性别:", this);
    m_genderCombo = new QComboBox(this);
    m_genderCombo->addItems({ "男", "女", "其他" });

    m_subscribeCheckBox = new QCheckBox("订阅新闻资讯", this);*/

    m_okButton = new QPushButton("确定", this);
    m_cancelButton = new QPushButton("取消", this);

    // 使用表单布局排列标签和输入控件
    QFormLayout* formLayout = new QFormLayout;
    formLayout->addRow(m_nameLabel, m_nameEdit);
    formLayout->addRow(m_nameLabel2, m_nameEdit2);
    formLayout->addRow(m_nameLabel3, m_nameEdit3);
    formLayout->addRow(m_nameLabel4, m_nameEdit4);
   /* formLayout->addRow(m_ageLabel, m_ageSpinBox);
    formLayout->addRow(m_genderLabel, m_genderCombo);
    formLayout->addRow(m_subscribeCheckBox);*/

    // 按钮布局
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);

    // 主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // 连接信号槽
    connect(m_okButton, &QPushButton::clicked, this, &MyDialog::onOkClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &MyDialog::onCancelClicked);
}

void MyDialog::onOkClicked()
{
    //// 简单验证
    //if (m_nameEdit->text().trimmed().isEmpty()) {
    //    QMessageBox::warning(this, "输入错误", "姓名不能为空！");
    //    return;
    //}

    //// 演示：获取数据并显示（可选）
    //QString name = m_nameEdit->text();
    //int age = m_ageSpinBox->value();
    //QString gender = m_genderCombo->currentText();
    //bool subscribe = m_subscribeCheckBox->isChecked();

    //// 在对话框中显示输入摘要（或者可以存入成员变量供主窗口访问）
    //QMessageBox::information(this, "提交成功",
    //    QString("姓名: %1\n年龄: %2\n性别: %3\n订阅: %4")
    //    .arg(name).arg(age).arg(gender).arg(subscribe ? "是" : "否"));

    accept(); // 关闭对话框并返回 QDialog::Accepted
}

void MyDialog::onCancelClicked()
{
    reject(); // 关闭对话框并返回 QDialog::Rejected
}