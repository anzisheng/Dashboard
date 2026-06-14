#include "MyDialog_press.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

MyDialog_press::MyDialog_press(QWidget* parent)
    : QDialog(parent)
{
    setupUI();
    setWindowTitle("压力工作参数：");
    setModal(true);  // 设置为模态对话框
    resize(350, 250);
}

MyDialog_press::~MyDialog_press()
{
 /*   delete m_nameLabel;
	delete m_nameEdit;

    delete m_nameLabel2;
    delete m_nameEdit2;
    delete PID_P;
	delete m_nameEditP;
    delete PID_I;
    delete m_nameEditI;
    delete PID_D;
    delete m_nameEditD;*/
}

void MyDialog_press::setupUI()
{
    // 创建控件
    m_nameLabel = new QLabel("压力告警高限:", this);
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("请输入压力告警高限值");

    m_nameLabel2 = new QLabel("压力告警低限:", this);
    m_nameEdit2 = new QLineEdit(this);
    m_nameEdit2->setPlaceholderText("请输入压力告警低限");

    PID_P = new QLabel("P参数:", this);
    m_nameEditP = new QLineEdit(this);
    m_nameEditP->setPlaceholderText("0.0");

    PID_I = new QLabel("I参数:", this);
    m_nameEditI = new QLineEdit(this);
    m_nameEditI->setPlaceholderText("0.0");

    PID_D = new QLabel("D参数:", this);
    m_nameEditD = new QLineEdit(this);
    m_nameEditD->setPlaceholderText("0.0");

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
    formLayout->addRow(PID_P, m_nameEditP);
    formLayout->addRow(PID_I, m_nameEditI);
    formLayout->addRow(PID_D, m_nameEditD);
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
    connect(m_okButton, &QPushButton::clicked, this, &MyDialog_press::onOkClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &MyDialog_press::onCancelClicked);
}

void MyDialog_press::onOkClicked()
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

void MyDialog_press::onCancelClicked()
{
    reject(); // 关闭对话框并返回 QDialog::Rejected
}