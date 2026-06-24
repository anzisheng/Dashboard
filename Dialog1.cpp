#include "Dialog1.h"
#include <QGraphicsTextItem>
#include < QDialogButtonBox >
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
#include <QGroupBox>
Dialog1::Dialog1(QWidget* parent)
    : QDialog(parent), m_currentEdit(nullptr)
{
    //setMinimumSize(300, 350);

        // 创建主布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(12);
    m_mainLayout->setContentsMargins(15, 15, 15, 15);

    // ============================================
// 顶部：图像显示 GroupBox（占一整行）
// ============================================
    m_imageGroupBox = new QGroupBox("Image Display", this);
    m_imageGroupBox->setStyleSheet(
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

    // 图像区域布局
    m_imageLayout = new QVBoxLayout(m_imageGroupBox);
    m_imageLayout->setSpacing(8);
    m_imageLayout->setContentsMargins(10, 10, 10, 10);

    // 创建 Graphics View 和 Scene
    m_graphicsView = new QGraphicsView(m_imageGroupBox);
    m_graphicsView->setMinimumHeight(200);
    m_graphicsView->setAlignment(Qt::AlignCenter);
    m_graphicsView->setBackgroundBrush(QBrush(Qt::lightGray));
    m_graphicsView->setFrameShape(QFrame::StyledPanel);
    m_graphicsView->setRenderHint(QPainter::Antialiasing);
    m_graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
    m_graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    m_graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_graphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    m_graphicsView->setStyleSheet(
        "QGraphicsView {"
        "    border: 1px solid #cccccc;"
        "    background-color: #f5f5f5;"
        "}"
    );
    // 创建场景
    setupImageScene();

    // 图像操作按钮布局
    m_imageButtonLayout = new QHBoxLayout();
    m_imageButtonLayout->setSpacing(6);

    m_loadImageBtn = new QPushButton("Load Image", m_imageGroupBox);
    //m_loadImageBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogStart));
    m_loadImageBtn->setMinimumHeight(28);

    m_clearImageBtn = new QPushButton("Clear", m_imageGroupBox);
    //m_clearImageBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogResetButton));
    m_clearImageBtn->setMinimumHeight(28);
    m_clearImageBtn->setEnabled(false);

    m_fitImageBtn = new QPushButton("Fit", m_imageGroupBox);
    //m_fitImageBtn->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    m_fitImageBtn->setMinimumHeight(28);
    m_fitImageBtn->setEnabled(false);

    m_imageButtonLayout->addWidget(m_loadImageBtn);
    m_imageButtonLayout->addWidget(m_clearImageBtn);
    m_imageButtonLayout->addWidget(m_fitImageBtn);
    m_imageButtonLayout->addStretch();


    // 将控件添加到图像布局
    m_imageLayout->addWidget(m_graphicsView);
    m_imageLayout->addLayout(m_imageButtonLayout);

    // 将图像GroupBox添加到主布局
    m_mainLayout->addWidget(m_imageGroupBox);


    // ============================================
    // 下部：水平布局，包含两个GroupBox
    // ============================================
    m_bottomLayout = new QHBoxLayout();
    m_bottomLayout->setSpacing(12);


    // ---------- 左侧：个人信息 GroupBox（5对） ----------
    m_personalInfoGroupBox = new QGroupBox("Personal Information", this);
    m_personalInfoGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_personalInfoGroupBox->setStyleSheet(
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


    // 创建网格布局用于个人信息
    m_personalInfoLayout = new QGridLayout(m_personalInfoGroupBox);
    m_personalInfoLayout->setSpacing(10);
    m_personalInfoLayout->setContentsMargins(15, 15, 15, 15);

    // 创建标签和输入框 - Name
    m_labelName = new QLabel("压力高限", m_personalInfoGroupBox);
    m_labelName->setMinimumWidth(80);
    m_labelName->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_fltEditPressAlmH = new QLineEdit(m_personalInfoGroupBox);
    m_fltEditPressAlmH->setPlaceholderText("输入压力高限");
    m_fltEditPressAlmH->setMinimumHeight(25);
	m_fltEditPressAlmH->installEventFilter(this); // 安装事件过滤器以捕获点击事件

    // 创建标签和输入框 - Email
    m_labelEmail = new QLabel("压力低限:", m_personalInfoGroupBox);
    m_labelEmail->setMinimumWidth(80);
    m_labelEmail->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_fltEditPressAlmL = new QLineEdit(m_personalInfoGroupBox);
    m_fltEditPressAlmL->setPlaceholderText("输入压力低限");
    m_fltEditPressAlmL->setMinimumHeight(25);
    m_fltEditPressAlmL->setInputMethodHints(Qt::ImhEmailCharactersOnly);
    m_fltEditPressAlmL->installEventFilter(this);

    // 创建标签和输入框 - Phone
    m_labelPhone = new QLabel("电话:", m_personalInfoGroupBox);
    m_labelPhone->setMinimumWidth(80);
    m_labelPhone->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_fltEditParaP = new QLineEdit(m_personalInfoGroupBox);
    m_fltEditParaP->setPlaceholderText("输入电话号码");
    m_fltEditParaP->setMinimumHeight(25);
    m_fltEditParaP->setInputMethodHints(Qt::ImhDialableCharactersOnly);
    m_fltEditParaP->installEventFilter(this);


    // 创建标签和输入框 - Birthday
    m_labelBirthday = new QLabel("生日:", m_personalInfoGroupBox);
    m_labelBirthday->setMinimumWidth(80);
    m_labelBirthday->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_fltEditParaI = new QLineEdit(m_personalInfoGroupBox);
    m_fltEditParaI->setPlaceholderText("YYYY-MM-DD");
    m_fltEditParaI->setMinimumHeight(25);
	m_fltEditParaI->installEventFilter(this);


    // 创建标签和输入框 - Occupation
    m_labelOccupation = new QLabel("Occupation:", m_personalInfoGroupBox);
    m_labelOccupation->setMinimumWidth(80);
    m_labelOccupation->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_lineEditOccupation = new QLineEdit(m_personalInfoGroupBox);
    m_lineEditOccupation->setPlaceholderText("Enter your occupation");
    m_lineEditOccupation->setMinimumHeight(25);

    // 将控件添加到网格布局（5行）
    m_personalInfoLayout->addWidget(m_labelName, 0, 0);
    m_personalInfoLayout->addWidget(m_fltEditPressAlmH, 0, 1);
    m_personalInfoLayout->addWidget(m_labelEmail, 1, 0);
    m_personalInfoLayout->addWidget(m_fltEditPressAlmL, 1, 1);
    m_personalInfoLayout->addWidget(m_labelPhone, 2, 0);
    m_personalInfoLayout->addWidget(m_fltEditParaP, 2, 1);
    m_personalInfoLayout->addWidget(m_labelBirthday, 3, 0);
    m_personalInfoLayout->addWidget(m_fltEditParaI, 3, 1);
    m_personalInfoLayout->addWidget(m_labelOccupation, 4, 0);
    m_personalInfoLayout->addWidget(m_lineEditOccupation, 4, 1);

    // 设置网格列比例
    m_personalInfoLayout->setColumnStretch(0, 0);
    m_personalInfoLayout->setColumnStretch(1, 1);


    // ---------- 右侧：地址信息 GroupBox（3对） ----------
    m_addressGroupBox = new QGroupBox("Address Information", this);
    m_addressGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_addressGroupBox->setStyleSheet(
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
    // 创建网格布局用于地址信息
    m_addressLayout = new QGridLayout(m_addressGroupBox);
    m_addressLayout->setSpacing(10);
    m_addressLayout->setContentsMargins(15, 15, 15, 15);

    // 创建标签和输入框 - Address
    m_labelAddress = new QLabel("Address:", m_addressGroupBox);
    m_labelAddress->setMinimumWidth(70);
    m_labelAddress->setAlignment(Qt::AlignRight | Qt::AlignVCenter);


    m_lineEditAddress = new QLineEdit(m_addressGroupBox);
    m_lineEditAddress->setPlaceholderText("Enter street address");
    m_lineEditAddress->setMinimumHeight(25);

    // 创建标签和输入框 - City
    m_labelCity = new QLabel("City:", m_addressGroupBox);
    m_labelCity->setMinimumWidth(70);
    m_labelCity->setAlignment(Qt::AlignRight | Qt::AlignVCenter);


    m_lineEditCity = new QLineEdit(m_addressGroupBox);
    m_lineEditCity->setPlaceholderText("Enter city");
    m_lineEditCity->setMinimumHeight(25);


    // 创建标签和输入框 - Zip Code
    m_labelZipCode = new QLabel("Zip Code:", m_addressGroupBox);
    m_labelZipCode->setMinimumWidth(70);
    m_labelZipCode->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_lineEditZipCode = new QLineEdit(m_addressGroupBox);
    m_lineEditZipCode->setPlaceholderText("Enter zip code");
    m_lineEditZipCode->setMinimumHeight(25);
    m_lineEditZipCode->setInputMethodHints(Qt::ImhDigitsOnly);

    // 将控件添加到网格布局（3行）
    m_addressLayout->addWidget(m_labelAddress, 0, 0);
    m_addressLayout->addWidget(m_lineEditAddress, 0, 1);
    m_addressLayout->addWidget(m_labelCity, 1, 0);
    m_addressLayout->addWidget(m_lineEditCity, 1, 1);
    m_addressLayout->addWidget(m_labelZipCode, 2, 0);
    m_addressLayout->addWidget(m_lineEditZipCode, 2, 1);

    // 设置网格列比例
    m_addressLayout->setColumnStretch(0, 0);
    m_addressLayout->setColumnStretch(1, 1);


    // 添加一些额外的行来平衡高度
    m_addressLayout->setRowStretch(3, 1);

    // 将两个GroupBox添加到底部水平布局
    m_bottomLayout->addWidget(m_personalInfoGroupBox, 2);
    m_bottomLayout->addWidget(m_addressGroupBox, 1);

    // 将底部布局添加到主布局
    m_mainLayout->addLayout(m_bottomLayout);

    // ============================================
  // 对话框按钮
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











    // 1. 创建控件
   /* m_nameEdit = new QLineEdit("anzs", this);
    m_emailEdit = new QLineEdit("anzisheng@163.com", this);*/

    // 2. 创建表单布局
    /*
    QFormLayout* layout = new QFormLayout(this);

    // 3. 用 addRow 方法添加行：左边标签，右边编辑框
   /* layout->addRow(tr("&姓名:"), m_nameEdit);
    layout->addRow(tr("邮箱:"), m_emailEdit);

    m_fltEditPressAlmH = new QLineEdit(this);
    m_fltEditPressAlmH->installEventFilter(this);
    layout->addRow(tr("&压力告警高限:"), m_fltEditPressAlmH);

    m_fltEditPressAlmL = new QLineEdit(this);
    //m_fltEditPressAlmH->setPlaceholderText(QStringLiteral("压力告警高限"));
    m_fltEditPressAlmL->installEventFilter(this);
    layout->addRow(tr("&压力告警低限:"), m_fltEditPressAlmL);

    m_fltEditParaP = new QLineEdit(this);// ->setPlaceholderText();
    //   layout->addWidget(m_fltEditParaP);
    m_fltEditParaP->installEventFilter(this);
    layout->addRow(tr("&P参数"), m_fltEditParaP);

    m_fltEditParaI = new QLineEdit(this);// ->setPlaceholderText();
    m_fltEditParaI->installEventFilter(this);
    //   layout->addWidget(m_fltEditParaP);
    //m_fltEditParaP->installEventFilter(this);
    layout->addRow(tr("&I参数"), m_fltEditParaI);

    m_fltEditParaD = new QLineEdit(this);// ->setPlaceholderText();
    //   layout->addWidget(m_fltEditParaP);
    //m_fltEditParaP->installEventFilter(this);
    m_fltEditParaD->installEventFilter(this);
    layout->addRow(tr("&D参数"), m_fltEditParaP);

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



/*
    m_okButton = new QPushButton(QStringLiteral("确定"), this);
    layout->addWidget(m_okButton);

    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 300, 150);

    connect(m_okButton, &QPushButton::clicked, this, &Dialog1::onOkClicked);*/

}
void Dialog1::setupImageScene()
{
    m_graphicsScene = new QGraphicsScene(this);
    m_graphicsScene->setBackgroundBrush(Qt::white);
    m_graphicsView->setScene(m_graphicsScene);

    // 添加一个占位文本
    QGraphicsTextItem* placeholder = m_graphicsScene->addText(
        "No Image Loaded\n\nClick 'Load Image'",
        QFont("Arial", 12)
    );
    placeholder->setDefaultTextColor(Qt::gray);
    placeholder->setPos(
        (m_graphicsView->width() - placeholder->boundingRect().width()) / 2,
        (m_graphicsView->height() - placeholder->boundingRect().height()) / 2
    );
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