#include "groupdialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

GroupDialog::GroupDialog(QWidget *parent)
    : QDialog(parent)
    , m_groupBox(nullptr)
    , m_label1(nullptr)
    , m_label2(nullptr)
    , m_label3(nullptr)
    , m_label4(nullptr)
    , m_lineEdit1(nullptr)
    , m_lineEdit2(nullptr)
    , m_lineEdit3(nullptr)
    , m_lineEdit4(nullptr)
    , m_checkBox(nullptr)
    , m_buttonBox(nullptr)
    , m_mainLayout(nullptr)
    , m_gridLayout(nullptr)
{
    setupUI();
    setupConnections();
    
    // 设置窗口属性
    setWindowTitle("Information Dialog");
    setModal(true);
    setMinimumSize(450, 300);

    // 初始化时更新按钮状态
    onInputChanged();
}

GroupDialog::~GroupDialog()
{
    // Qt会自动清理子控件
}

void GroupDialog::setupUI()
{
    // 创建主布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(12);
    m_mainLayout->setContentsMargins(15, 15, 15, 15);

    // ============================================
    // 创建 GroupBox
    // ============================================
    m_groupBox = new QGroupBox("输入液位信息", this);
    m_groupBox->setStyleSheet(
        "QGroupBox {"
        "    font-weight: bold;"
        "    border: 2px solid #8f8f8f;"
        "    border-radius: 5px;"
        "    margin-top: 10px;"
        "    padding-top: 10px;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 5px 0 5px;"
        "}"
    );

    // 创建网格布局用于GroupBox内部
    m_gridLayout = new QGridLayout(m_groupBox);
    m_gridLayout->setSpacing(10);
    m_gridLayout->setContentsMargins(15, 15, 15, 15);

    // ---------- 第一对：Field 1 ----------
    m_label1 = new QLabel("液位高限报警阈值:", m_groupBox);
    m_label1->setMinimumWidth(70);
    m_label1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    m_lineEdit1 = new QLineEdit(m_groupBox);
    m_lineEdit1->setPlaceholderText("高限报警阈值");
    m_lineEdit1->setMinimumHeight(25);

    // ---------- 第二对：Field 2 ----------
    m_label2 = new QLabel("Field 2:", m_groupBox);
    m_label2->setMinimumWidth(70);
    m_label2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    m_lineEdit2 = new QLineEdit(m_groupBox);
    m_lineEdit2->setPlaceholderText("Enter field 2");
    m_lineEdit2->setMinimumHeight(25);

    // ---------- 第三对：Field 3 ----------
    m_label3 = new QLabel("Field 3:", m_groupBox);
    m_label3->setMinimumWidth(70);
    m_label3->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    m_lineEdit3 = new QLineEdit(m_groupBox);
    m_lineEdit3->setPlaceholderText("Enter field 3");
    m_lineEdit3->setMinimumHeight(25);

    // ---------- 第四对：Field 4 ----------
    m_label4 = new QLabel("Field 4:", m_groupBox);
    m_label4->setMinimumWidth(70);
    m_label4->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    m_lineEdit4 = new QLineEdit(m_groupBox);
    m_lineEdit4->setPlaceholderText("Enter field 4");
    m_lineEdit4->setMinimumHeight(25);

    // 将控件添加到网格布局（4行）
    m_gridLayout->addWidget(m_label1, 0, 0);
    m_gridLayout->addWidget(m_lineEdit1, 0, 1);
    m_gridLayout->addWidget(m_label2, 1, 0);
    m_gridLayout->addWidget(m_lineEdit2, 1, 1);
    m_gridLayout->addWidget(m_label3, 2, 0);
    m_gridLayout->addWidget(m_lineEdit3, 2, 1);
    m_gridLayout->addWidget(m_label4, 3, 0);
    m_gridLayout->addWidget(m_lineEdit4, 3, 1);

    // 设置网格列比例
    m_gridLayout->setColumnStretch(0, 0);
    m_gridLayout->setColumnStretch(1, 1);

    // 将GroupBox添加到主布局
    m_mainLayout->addWidget(m_groupBox);

    // ============================================
    // 创建 CheckBox
    // ============================================
    m_checkBox = new QCheckBox("Enable additional options", this);
    m_checkBox->setStyleSheet(
        "QCheckBox {"
        "    spacing: 8px;"
        "    font-weight: normal;"
        "}"
        "QCheckBox::indicator {"
        "    width: 18px;"
        "    height: 18px;"
        "}"
    );

    // 添加到主布局
    m_mainLayout->addWidget(m_checkBox);

    // ============================================
    // 创建对话框按钮
    // ============================================
    m_buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        this
    );
    
    m_buttonBox->setStyleSheet(
        "QPushButton {"
        "    padding: 6px 25px;"
        "    min-height: 28px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e0e0e0;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #c0c0c0;"
        "}"
    );

    // 添加到主布局
    m_mainLayout->addWidget(m_buttonBox);
}

void GroupDialog::setupConnections()
{
    // 连接输入变化信号
    connect(m_lineEdit1, &QLineEdit::textChanged, 
            this, &GroupDialog::onInputChanged);
    connect(m_lineEdit2, &QLineEdit::textChanged, 
            this, &GroupDialog::onInputChanged);
    connect(m_lineEdit3, &QLineEdit::textChanged, 
            this, &GroupDialog::onInputChanged);
    connect(m_lineEdit4, &QLineEdit::textChanged, 
            this, &GroupDialog::onInputChanged);
    connect(m_checkBox, &QCheckBox::stateChanged, 
            this, &GroupDialog::onInputChanged);

    // 连接对话框按钮
    connect(m_buttonBox, &QDialogButtonBox::accepted, 
            this, &QDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, 
            this, &QDialog::reject);
}

// ============================================
// Getter 方法
// ============================================

QString GroupDialog::getField1() const
{
    return m_lineEdit1->text().trimmed();
}

QString GroupDialog::getField2() const
{
    return m_lineEdit2->text().trimmed();
}

QString GroupDialog::getField3() const
{
    return m_lineEdit3->text().trimmed();
}

QString GroupDialog::getField4() const
{
    return m_lineEdit4->text().trimmed();
}

bool GroupDialog::getCheckBoxState() const
{
    return m_checkBox->isChecked();
}

// ============================================
// Setter 方法
// ============================================

void GroupDialog::setField1(const QString &value)
{
    m_lineEdit1->setText(value);
}

void GroupDialog::setField2(const QString &value)
{
    m_lineEdit2->setText(value);
}

void GroupDialog::setField3(const QString &value)
{
    m_lineEdit3->setText(value);
}

void GroupDialog::setField4(const QString &value)
{
    m_lineEdit4->setText(value);
}

void GroupDialog::setCheckBoxState(bool checked)
{
    m_checkBox->setChecked(checked);
}

void GroupDialog::clearInputs()
{
    m_lineEdit1->clear();
    m_lineEdit2->clear();
    m_lineEdit3->clear();
    m_lineEdit4->clear();
    m_checkBox->setChecked(false);
}

// ============================================
// 槽函数
// ============================================

void GroupDialog::onInputChanged()
{
    // 验证所有4个字段是否都已填写
    bool allFilled = !m_lineEdit1->text().trimmed().isEmpty() &&
                     !m_lineEdit2->text().trimmed().isEmpty() &&
                     !m_lineEdit3->text().trimmed().isEmpty() &&
                     !m_lineEdit4->text().trimmed().isEmpty();

    // 获取OK按钮并启用/禁用
    QPushButton *okButton = m_buttonBox->button(QDialogButtonBox::Ok);
    if (okButton) {
        okButton->setEnabled(allFilled);
    }

    // 根据CheckBox状态输出调试信息
    if (m_checkBox->isChecked()) {
        qDebug() << "CheckBox is checked - additional options enabled";
    } else {
        qDebug() << "CheckBox is unchecked - additional options disabled";
    }
}