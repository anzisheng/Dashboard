#include "Dialog1.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include "NumpadDialog.h"
#include <QFormLayout>
Dialog1::Dialog1(QWidget* parent)
    : QDialog(parent), m_currentEdit(nullptr)
{
    setMinimumSize(300, 350);

    // 1. 创建控件
   /* m_nameEdit = new QLineEdit("anzs", this);
    m_emailEdit = new QLineEdit("anzisheng@163.com", this);*/

    // 2. 创建表单布局
    QFormLayout* layout = new QFormLayout(this);

    // 3. 用 addRow 方法添加行：左边标签，右边编辑框
   /* layout->addRow(tr("&姓名:"), m_nameEdit);
    layout->addRow(tr("邮箱:"), m_emailEdit);*/

    m_fltEditPressAlmH = new QLineEdit(this);
    layout->addRow(tr("&压力告警高限:"), m_fltEditPressAlmH);

    m_fltEditPressAlmL = new QLineEdit(this);
    //m_fltEditPressAlmH->setPlaceholderText(QStringLiteral("压力告警高限"));
    layout->addRow(tr("&压力告警低限:"), m_fltEditPressAlmL);

    m_fltEditParaP = new QLineEdit(this);// ->setPlaceholderText();
    //   layout->addWidget(m_fltEditParaP);
    //m_fltEditParaP->installEventFilter(this);
    layout->addRow(tr("&P参数"), m_fltEditParaP);

    m_fltEditParaI = new QLineEdit(this);// ->setPlaceholderText();
    //   layout->addWidget(m_fltEditParaP);
    //m_fltEditParaP->installEventFilter(this);
    layout->addRow(tr("&I参数"), m_fltEditParaI);

    m_fltEditParaD = new QLineEdit(this);// ->setPlaceholderText();
    //   layout->addWidget(m_fltEditParaP);
    //m_fltEditParaP->installEventFilter(this);
    layout->addRow(tr("&D参数"), m_fltEditParaD);

/*

    setWindowTitle(QStringLiteral("对话框 1"));
    setMinimumSize(300, 450);

    auto* layout = new QVBoxLayout(this);
    QFormLayout* formLayout = new QFormLayout(this);

    //auto* label = new QLabel(QStringLiteral("设置压力工作参数:"), this);
    //label->setAlignment(Qt::AlignCenter);
    //layout->addWidget(label);
    //layout->setSpacing(-2);


    m_fltLabelPressAlmH = new QLabel(this);
    //layout->addWidget(m_fltLabelPressAlmH);
    //m_fltLabelPressAlmH->setGeometry(10, 10, 100, 30);
    m_fltEditPressAlmH = new QLineEdit(this);
    //m_fltEditPressAlmH->setPlaceholderText(QStringLiteral("压力告警高限"));
    //layout->addWidget(m_fltEditPressAlmH);
   // m_fltEditPressAlmH->installEventFilter(this);
    formLayout->addRow(tr("&姓名:"), m_fltEditPressAlmH);
    //formLayout->addRow(tr("&邮箱:"), m_fltEditPressAlmH);

    //formLayout->addRow("&压力告警高限:", m_fltEditPressAlmH);
    //layout->setSpacing(30);*/
    /*

    m_fltLabelPressAlmL = new QLabel(QStringLiteral("压力告警低限"), this);
    //layout->addWidget(m_fltLabelPressAlmL);
    m_fltLabelPressAlmL->setGeometry(10, 10, 100, 30);
    m_fltEditPressAlmL = new QLineEdit(this);
    m_fltEditPressAlmL->setPlaceholderText(QStringLiteral("压力告警低限"));
    //layout->addWidget(m_fltEditPressAlmL);
    m_fltEditPressAlmL->installEventFilter(this);
    formLayout->addRow(m_fltLabelPressAlmL, m_fltEditPressAlmL);
   


    m_fltEditPressAlmL = new QLineEdit(this);
    m_fltEditPressAlmL->setPlaceholderText(QStringLiteral("压力告警低限"));
    layout->addWidget(m_fltEditPressAlmL);
    m_fltEditPressAlmL->installEventFilter(this);*/

    /*
    m_fltLabelParaP = new QLabel(QStringLiteral("P参数"), this);
    layout->addWidget(m_fltLabelParaP);
    m_fltLabelParaP->setGeometry(10, 10, 100, 30);
    m_fltEditParaP = new QLineEdit(this);
    m_fltEditParaP->setPlaceholderText(QStringLiteral("P参数"));
 //   layout->addWidget(m_fltEditParaP);
    m_fltEditParaP->installEventFilter(this);
    formLayout->addRow(m_fltLabelParaP, m_fltEditParaP);

    m_fltLabelParaI = new QLabel(QStringLiteral("I参数"), this);
    layout->addWidget(m_fltLabelParaI);
    m_fltLabelParaI->setGeometry(10, 10, 100, 30);
    m_fltEditParaD = new QLineEdit(this);
    m_fltEditParaD->setPlaceholderText(QStringLiteral("I参数"));
    m_fltEditParaD->setGeometry(10, 10, 100, 30);
    formLayout->addRow(m_fltLabelParaI, m_fltEditParaD);
   // layout->addWidget(m_fltEditParaD);

    m_fltEditParaP->installEventFilter(this);

    m_fltEditParaI = new QLineEdit(this);
    m_fltEditParaI->setPlaceholderText(QStringLiteral("I参数"));
    layout->addWidget(m_fltEditParaI);
    m_fltEditParaI->installEventFilter(this);
    m_fltEditParaI->installEventFilter(this);

    formLayout->addRow(m_fltLabelParaI, m_fltEditParaI);

    m_fltEditParaD = new QLineEdit(this);
    m_fltEditParaD->setPlaceholderText(QStringLiteral("D参数"));
    layout->addWidget(m_fltEditParaD);
    m_fltEditParaD->installEventFilter(this);
    m_fltEditParaD->installEventFilter(this);

    formLayout->addRow(m_fltLabelParaI, m_fltEditParaD); */

    m_okButton = new QPushButton(QStringLiteral("确定"), this);
    layout->addWidget(m_okButton);

    connect(m_okButton, &QPushButton::clicked, this, &Dialog1::onOkClicked);

}

bool Dialog1::eventFilter(QObject* obj, QEvent* event)
{

    if (event->type() == QEvent::MouseButtonPress) {
        // 判断点击的是否是我们关心的编辑框
        if (obj == m_fltEditPressAlmH || obj == m_fltEditPressAlmL|| obj == m_fltEditParaP || obj == m_fltEditParaI || obj == m_fltEditParaD)
        {
            m_currentEdit = qobject_cast<QLineEdit*>(obj);  // 记录当前编辑框
            onLineEditClicked();
            return true;   // 事件已处理，阻止默认行为（焦点移动）
        }
    }
    return QDialog::eventFilter(obj, event);

}
/*
bool Dialog1::eventFilter(QObject* obj, QEvent* event)
{
    // 捕获目标编辑框的鼠标点击事件
    if (obj == m_fltEditPressAlmH && event->type() == QEvent::MouseButtonPress) {
        onLineEditClicked();
        return true;   // 事件已处理，不再传递（避免焦点变化）
    }
    return QDialog::eventFilter(obj, event);
}*/

void Dialog1::onLineEditClicked()
{
    if (!m_currentEdit) return;  // 安全保护



    // 延迟创建对话框（只创建一次）
    if (!m_dialog) {
        m_dialog = new NumPadDialog(this);
        connect(m_dialog, &QDialog::accepted, this, &Dialog1::onDialogAccepted);
    }

    // 将当前编辑框的内容作为初始文本
    m_dialog->setText(m_currentEdit->text());

    // 模态弹出数字键盘
    m_dialog->exec();
}

void Dialog1::onDialogAccepted()
{
    if (!m_currentEdit) return;
    // 从对话框获取输入结果，回填到编辑框
     // 从对话框获取输入，回填到当前编辑框
    QString newText = m_dialog->getText();
    m_currentEdit->setText(newText);
}

void Dialog1::onOkClicked()
{
   // qDebug() << "对话框1 输入内容:" << m_lineEdit->text();

    accept();
}