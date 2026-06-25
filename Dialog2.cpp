#include "Dialog2.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QGroupBox>
#include <QCheckBox>
#include <QDialogButtonBox>

Dialog2::Dialog2(QWidget* parent)
    : QDialog(parent), m_dialog(nullptr)
{
    setupUI();
    setupConnections();

    // 设置窗口属性
    setWindowTitle("Information Dialog");
    setModal(true);
    setMinimumSize(450, 300);
    //setWindowTitle(QStringLiteral("对话框 2"));
    //setMinimumSize(300, 250);

    /*

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
    layout->addWidget(m_fltEditYwWorkL);*/

   /* m_okButton = new QPushButton(QStringLiteral("确定"), this);
    layout->addWidget(m_okButton);

    connect(m_okButton, &QPushButton::clicked, this, &Dialog2::onOkClicked);*/
}

void Dialog2::setupUI()
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
    m_lineEdit1->setPlaceholderText("输入液位高限报警阈值");
    m_lineEdit1->setMinimumHeight(25);
	m_lineEdit1->installEventFilter(this); // 安装事件过滤器


    // ---------- 第二对：Field 2 ----------
    m_label2 = new QLabel("液位低限报警阈值:", m_groupBox);
    m_label2->setMinimumWidth(70);
    m_label2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_lineEdit2 = new QLineEdit(m_groupBox);
    m_lineEdit2->setPlaceholderText("输入液位低限报警阈值");
    m_lineEdit2->setMinimumHeight(25);
    m_lineEdit2->installEventFilter(this); // 安装事件过滤器


    // ---------- 第三对：Field 3 ----------
    m_label3 = new QLabel("液位高限停止值", m_groupBox);
    m_label3->setMinimumWidth(70);
    m_label3->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_lineEdit3 = new QLineEdit(m_groupBox);
    m_lineEdit3->setPlaceholderText("液位高限停止值");
    m_lineEdit3->setMinimumHeight(25);
    m_lineEdit3->installEventFilter(this); // 安装事件过滤器


    // ---------- 第四对：Field 4 ----------
    m_label4 = new QLabel("液位低限启动值", m_groupBox);
    m_label4->setMinimumWidth(70);
    m_label4->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_lineEdit4 = new QLineEdit(m_groupBox);
    m_lineEdit4->setPlaceholderText("液位低限启动值");
    m_lineEdit4->setMinimumHeight(25);
    m_lineEdit4->installEventFilter(this); // 安装事件过滤器



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

void Dialog2::setupConnections()
{
    // 连接输入变化信号
    /*
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
        */
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
            obj == m_fltEditYwAlmL  ||
            obj == m_lineEdit1 || obj == m_lineEdit2 || obj == m_lineEdit3 || obj == m_lineEdit4) {
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