#include "Dialog3.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

Dialog3::Dialog3(QWidget* parent)
    : QDialog(parent)
{
    setupUI();
    setWindowTitle("DMX Dialog");
    resize(400, 200);
 //   auto* layout = new QVBoxLayout(this);

 //   auto* label = new QLabel(QStringLiteral("请选择DMX输入"), this);
 //   label->setAlignment(Qt::AlignCenter);
 //   layout->addWidget(label);

 //   m_bCheckStopIn = new QCheckBox("急停",this);
 //   //m_lineEdit->setPlaceholderText(QStringLiteral("请输入内容..."));
 //   layout->addWidget(m_bCheckStopIn);

 //   m_bCheckOverPressure = new QCheckBox("过压报警", this);// nullptr;//过压报警
 //   layout->addWidget(m_bCheckOverPressure);

 //    m_bCheckRelief = new QCheckBox("泄压阀", this);// nullptr;//泄压阀
 //   layout->addWidget(m_bCheckRelief);

 //   m_bCheckBeep = new QCheckBox("蜂鸣器", this);// nullptr;//泄压阀
 //   layout->addWidget(m_bCheckBeep);

 //   m_bCheckPump = new QCheckBox("补油泵", this);// nullptr;//泄压阀
	//layout->addWidget(m_bCheckPump);

 //   m_okButton = new QPushButton(QStringLiteral("确定"), this);
 //   layout->addWidget(m_okButton);

 //   connect(m_okButton, &QPushButton::clicked, this, &Dialog3::onOkClicked);
}

void Dialog3::setupUI()
{
    // 创建主布局
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 创建DMX输入GroupBox
    dmxGroupBox = new QGroupBox("DMX输入", this);
    QGridLayout* gridLayout = new QGridLayout(dmxGroupBox);

    // 第一行：首地址
    addressLabel = new QLabel("首地址：", dmxGroupBox);
    addressLineEdit = new QLineEdit(dmxGroupBox);
    addressLineEdit->setPlaceholderText("输入首地址");

    // 添加第一行到网格布局 (行0)
    gridLayout->addWidget(addressLabel, 0, 0, Qt::AlignRight | Qt::AlignVCenter);
    gridLayout->addWidget(addressLineEdit, 0, 1, 1, 3);

    // 第二行：CH1-CH4
    ch1Label = new QLabel("CH1", dmxGroupBox);
    ch2Label = new QLabel("CH2", dmxGroupBox);
    ch3Label = new QLabel("CH3", dmxGroupBox);
    ch4Label = new QLabel("CH4", dmxGroupBox);

    ch1LineEdit = new QLineEdit(dmxGroupBox);
    ch2LineEdit = new QLineEdit(dmxGroupBox);
    ch3LineEdit = new QLineEdit(dmxGroupBox);
    ch4LineEdit = new QLineEdit(dmxGroupBox);

    // 设置占位文本
    ch1LineEdit->setPlaceholderText("0");
    ch2LineEdit->setPlaceholderText("0");
    ch3LineEdit->setPlaceholderText("0");
    ch4LineEdit->setPlaceholderText("0");

    // 设置固定宽度
    ch1LineEdit->setFixedWidth(60);
    ch2LineEdit->setFixedWidth(60);
    ch3LineEdit->setFixedWidth(60);
    ch4LineEdit->setFixedWidth(60);

    // 添加第二行到网格布局 (行1)
    gridLayout->addWidget(ch1Label, 1, 0, Qt::AlignCenter);
    gridLayout->addWidget(ch2Label, 1, 1, Qt::AlignCenter);
    gridLayout->addWidget(ch3Label, 1, 2, Qt::AlignCenter);
    gridLayout->addWidget(ch4Label, 1, 3, Qt::AlignCenter);

    gridLayout->addWidget(ch1LineEdit, 2, 0, Qt::AlignCenter);
    gridLayout->addWidget(ch2LineEdit, 2, 1, Qt::AlignCenter);
    gridLayout->addWidget(ch3LineEdit, 2, 2, Qt::AlignCenter);
    gridLayout->addWidget(ch4LineEdit, 2, 3, Qt::AlignCenter);

    // 设置网格列比例
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    gridLayout->setColumnStretch(2, 1);
    gridLayout->setColumnStretch(3, 1);

    // 设置GroupBox的布局
    dmxGroupBox->setLayout(gridLayout);

    // 添加GroupBox到主布局
    mainLayout->addWidget(dmxGroupBox);

    // 创建按钮布局（水平布局，使按钮右对齐）
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(); // 添加弹性空间

    // 创建退出按钮
    exitButton = new QPushButton("退出", this);
    exitButton->setFixedWidth(80);
    buttonLayout->addWidget(exitButton);

    // 添加按钮布局到主布局
    mainLayout->addLayout(buttonLayout);

    // 连接退出按钮的点击信号到对话框的accept槽
    connect(exitButton, &QPushButton::clicked, this, &QDialog::accept);
}

//void Dialog3::onOkClicked()
//{
//    //qDebug() << "对话框1 输入内容:" << m_lineEdit->text();
//    accept();
//}