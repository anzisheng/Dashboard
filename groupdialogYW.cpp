#include "groupdialogYW.h"

#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QMessageBox>
#include <QApplication>
#include <QPen>
#include <QPainterPath>
#include <QPolygonF>
#include <QRandomGenerator>
#include <cmath>
#include "NumpadDialog.h"

void GroupDialogYW::onDialogAccepted()
{
    if (!m_currentEdit) return;
    // 从对话框获取输入结果，回填到编辑框
     // 从对话框获取输入，回填到当前编辑框
    QString newText = m_dialog->getText();
    m_currentEdit->setText(newText);
}


GroupDialogYW::GroupDialogYW(QWidget* parent)
    : QDialog(parent)
    , m_imageGroupBox(nullptr)
    , m_graphicsView(nullptr)
    , m_graphicsScene(nullptr)
    , m_startStopBtn(nullptr)
    , m_writeButton(nullptr)
    , m_personalInfoGroupBox(nullptr)
    , m_labelName(nullptr)
    , m_labelEmail(nullptr)
    , m_labelPhone(nullptr)
    , m_labelBirthday(nullptr)
    , m_labelOccupation(nullptr)
    , m_lineEditName(nullptr)
    , m_lineEditEmail(nullptr)
    , m_lineEditPhone(nullptr)
    , m_lineEditBirthday(nullptr)
    , m_lineEditOccupation(nullptr)
    , m_addressGroupBox(nullptr)
    , m_labelAddress(nullptr)
    , m_labelCity(nullptr)
    , m_labelZipCode(nullptr)
    , m_lineEditAddress(nullptr)
    , m_lineEditCity(nullptr)
    , m_lineEditZipCode(nullptr)
    , m_buttonBox(nullptr)
    , m_mainLayout(nullptr)
    , m_bottomLayout(nullptr)
    , m_imageLayout(nullptr)
    , m_imageButtonLayout(nullptr)
    , m_personalInfoLayout(nullptr)
    , m_addressLayout(nullptr)
    , m_timer(nullptr)
    , m_isRunning(false)
    , m_maxDataPoints(100)
    , m_currentTime(0.0)
    , m_displayDuration(30.0)
    , m_coordinateSystemDrawn(false)
{
    // 先创建定时器
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);

    // 创建UI
    setupUI();

    // 建立连接
    setupConnections();

    setWindowTitle("压力实时曲线图");
    setModal(true);
    setMinimumSize(700, 600);

    onInputChanged();

    // 生成初始数据
    generateInitialData();

    // 绘制坐标系（只绘制一次）
    drawCoordinateSystem();

    // 绘制曲线
    drawCurves();

    // 更新按钮状态 - "读取"按钮始终可用
    onInputChanged();
}

GroupDialogYW::~GroupDialogYW()
{
    if (m_timer) {
        m_timer->stop();
    }
}

void GroupDialogYW::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(10);
    m_mainLayout->setContentsMargins(10, 10, 10, 10);

    // ============================================
    // 顶部：图像显示 GroupBox
    // ============================================
    m_imageGroupBox = new QGroupBox("Real-time Plot", this);

    m_imageLayout = new QVBoxLayout(m_imageGroupBox);
    m_imageLayout->setSpacing(6);
    m_imageLayout->setContentsMargins(8, 8, 8, 8);

    m_graphicsView = new QGraphicsView(m_imageGroupBox);
    m_graphicsView->setMinimumHeight(250);
    m_graphicsView->setAlignment(Qt::AlignCenter);
    m_graphicsView->setBackgroundBrush(QBrush(Qt::white));
    m_graphicsView->setFrameShape(QFrame::StyledPanel);
    m_graphicsView->setRenderHint(QPainter::Antialiasing);
    m_graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);

    setupImageScene();

    // ---------- 图像操作按钮布局（只保留 Start/Stop 按钮） ----------
    m_imageButtonLayout = new QHBoxLayout();
    m_imageButtonLayout->setSpacing(6);

    m_startStopBtn = new QPushButton("开始", m_imageGroupBox);
    m_startStopBtn->setMinimumHeight(26);
    m_startStopBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");

    m_imageButtonLayout->addWidget(m_startStopBtn);
    m_imageButtonLayout->addStretch();

    m_imageLayout->addWidget(m_graphicsView);
    m_imageLayout->addLayout(m_imageButtonLayout);

    m_mainLayout->addWidget(m_imageGroupBox);

    // ============================================
    // 下部：水平布局，包含两个GroupBox
    // ============================================
    m_bottomLayout = new QHBoxLayout();
    m_bottomLayout->setSpacing(10);

    // ---------- 左侧：个人信息 GroupBox（5对） ----------
    m_personalInfoGroupBox = new QGroupBox("压力参数", this);
    m_personalInfoGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_personalInfoLayout = new QGridLayout(m_personalInfoGroupBox);
    m_personalInfoLayout->setSpacing(8);
    m_personalInfoLayout->setContentsMargins(10, 10, 10, 10);

    m_labelName = new QLabel("压力高限:", m_personalInfoGroupBox);
    m_labelName->setMinimumWidth(70);
    m_labelName->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lineEditName = new QLineEdit(m_personalInfoGroupBox);
    m_lineEditName->setPlaceholderText("输入压力高限");
    m_lineEditName->installEventFilter(this);
    m_labelEmail = new QLabel("压力低限:", m_personalInfoGroupBox);
    m_labelEmail->setMinimumWidth(70);
    m_labelEmail->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lineEditEmail = new QLineEdit(m_personalInfoGroupBox);
    m_lineEditEmail->setPlaceholderText("输入压力低限");
	m_lineEditEmail->installEventFilter(this);


    m_labelPhone = new QLabel("P参数:", m_personalInfoGroupBox);
    m_labelPhone->setMinimumWidth(70);
    m_labelPhone->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lineEditPhone = new QLineEdit(m_personalInfoGroupBox);
    m_lineEditPhone->setPlaceholderText("输入P参数");
	m_lineEditPhone->installEventFilter(this);

    m_labelBirthday = new QLabel("I参数:", m_personalInfoGroupBox);
    m_labelBirthday->setMinimumWidth(70);
    m_labelBirthday->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lineEditBirthday = new QLineEdit(m_personalInfoGroupBox);
    m_lineEditBirthday->setPlaceholderText("输入I参数");
	m_lineEditBirthday->installEventFilter(this);

    m_labelOccupation = new QLabel("D参数",m_personalInfoGroupBox);
    m_labelOccupation->setMinimumWidth(70);
    m_labelOccupation->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lineEditOccupation = new QLineEdit(m_personalInfoGroupBox);
    m_lineEditOccupation->setPlaceholderText("输入D参数");
	m_lineEditOccupation->installEventFilter(this);

    m_personalInfoLayout->addWidget(m_labelName, 0, 0);
    m_personalInfoLayout->addWidget(m_lineEditName, 0, 1);
    m_personalInfoLayout->addWidget(m_labelEmail, 1, 0);
    m_personalInfoLayout->addWidget(m_lineEditEmail, 1, 1);
    m_personalInfoLayout->addWidget(m_labelPhone, 2, 0);
    m_personalInfoLayout->addWidget(m_lineEditPhone, 2, 1);
    m_personalInfoLayout->addWidget(m_labelBirthday, 3, 0);
    m_personalInfoLayout->addWidget(m_lineEditBirthday, 3, 1);
    m_personalInfoLayout->addWidget(m_labelOccupation, 4, 0);
    m_personalInfoLayout->addWidget(m_lineEditOccupation, 4, 1);

    m_personalInfoLayout->setColumnStretch(0, 0);
    m_personalInfoLayout->setColumnStretch(1, 1);

    // ---------- 右侧：地址信息 GroupBox（3对） ----------
    m_addressGroupBox = new QGroupBox("误差信息", this);
    m_addressGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_addressLayout = new QGridLayout(m_addressGroupBox);
    m_addressLayout->setSpacing(8);
    m_addressLayout->setContentsMargins(10, 10, 10, 10);

    m_labelAddress = new QLabel("偏差容限:", m_addressGroupBox);
    m_labelAddress->setMinimumWidth(70);
    m_labelAddress->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lineEditAddress = new QLineEdit(m_addressGroupBox);
    m_lineEditAddress->setPlaceholderText("偏差容限");
	m_lineEditAddress->installEventFilter(this);

    m_labelCity = new QLabel("积分偏差:", m_addressGroupBox);
    m_labelCity->setMinimumWidth(70);
    m_labelCity->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lineEditCity = new QLineEdit(m_addressGroupBox);
    m_lineEditCity->setPlaceholderText("积分偏差");
	m_lineEditCity->installEventFilter(this);

    m_labelZipCode = new QLabel("设定压力:", m_addressGroupBox);
    m_labelZipCode->setMinimumWidth(70);
    m_labelZipCode->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_lineEditZipCode = new QLineEdit(m_addressGroupBox);
    m_lineEditZipCode->setPlaceholderText("设定压力");
	m_lineEditZipCode->installEventFilter(this);

    m_addressLayout->addWidget(m_labelAddress, 0, 0);
    m_addressLayout->addWidget(m_lineEditAddress, 0, 1);
    m_addressLayout->addWidget(m_labelCity, 1, 0);
    m_addressLayout->addWidget(m_lineEditCity, 1, 1);
    m_addressLayout->addWidget(m_labelZipCode, 2, 0);
    m_addressLayout->addWidget(m_lineEditZipCode, 2, 1);

    m_addressLayout->setColumnStretch(0, 0);
    m_addressLayout->setColumnStretch(1, 1);
    m_addressLayout->setRowStretch(3, 1);

    m_bottomLayout->addWidget(m_personalInfoGroupBox, 2);
    m_bottomLayout->addWidget(m_addressGroupBox, 1);

    m_mainLayout->addLayout(m_bottomLayout);

    // ============================================
    // 对话框按钮
    // ============================================
    m_buttonBox = new QDialogButtonBox(Qt::Horizontal, this);

    // 创建 "读取" 按钮
    QPushButton* readButton = new QPushButton("读取", this);
    readButton->setMinimumHeight(28);
    readButton->setMinimumWidth(60);
    readButton->setStyleSheet(
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

    // 创建 "写入" 按钮
    m_writeButton = new QPushButton("写入", this);
    m_writeButton->setMinimumHeight(28);
    m_writeButton->setMinimumWidth(60);
    m_writeButton->setStyleSheet(
        "QPushButton {"
        "    padding: 6px 25px;"
        "    min-height: 28px;"
        "    font-weight: bold;"
        "    background-color: #4CAF50;"
        "    color: white;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3d8b40;"
        "}"
    );

    // 创建 "放弃" 按钮
    QPushButton* cancelButton = new QPushButton("放弃", this);
    cancelButton->setMinimumHeight(28);
    cancelButton->setMinimumWidth(60);
    cancelButton->setStyleSheet(
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

    // 添加按钮到按钮框
    m_buttonBox->addButton(readButton, QDialogButtonBox::AcceptRole);
    m_buttonBox->addButton(m_writeButton, QDialogButtonBox::ActionRole);
    m_buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

    // 连接信号
    connect(readButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    // 添加到主布局
    m_mainLayout->addWidget(m_buttonBox);
}

void GroupDialogYW::setupImageScene()
{
    m_graphicsScene = new QGraphicsScene(this);
    m_graphicsScene->setBackgroundBrush(Qt::white);
    m_graphicsView->setScene(m_graphicsScene);
}

bool GroupDialogYW::eventFilter(QObject* obj, QEvent* event)
{

    if (event->type() == QEvent::MouseButtonPress) {
        // 判断点击的是否是我们关心的编辑框
        if (/*obj == m_fltEditPressAlmH || obj == m_fltEditPressAlmL || obj == m_fltEditParaP || obj == m_fltEditParaI || obj == m_fltEditParaD*/
            obj == m_lineEditName || obj == m_lineEditEmail || obj == m_lineEditOccupation
			||obj == m_lineEditBirthday || obj == m_lineEditPhone
            || obj == m_lineEditAddress ||obj == m_lineEditCity || obj == m_lineEditZipCode)
        {
            m_currentEdit = qobject_cast<QLineEdit*>(obj);  // 记录当前编辑框
            onLineEditClicked();
            return true;   // 事件已处理，阻止默认行为（焦点移动）
        }
    }
    return QDialog::eventFilter(obj, event);

}
void GroupDialogYW::onLineEditClicked()
{
    if (!m_currentEdit) return;  // 安全保护



    // 延迟创建对话框（只创建一次）
    if (!m_dialog) {
        m_dialog = new NumPadDialog(this);
        connect(m_dialog, &QDialog::accepted, this, &GroupDialogYW::onDialogAccepted);
    }

    // 将当前编辑框的内容作为初始文本
    m_dialog->setText(m_currentEdit->text());

    // 模态弹出数字键盘
    m_dialog->exec();
}



void GroupDialogYW::setupConnections()
{
    // 连接输入变化信号
    connect(m_lineEditName, &QLineEdit::textChanged, this, &GroupDialogYW::onInputChanged);
    connect(m_lineEditEmail, &QLineEdit::textChanged, this, &GroupDialogYW::onInputChanged);
    connect(m_lineEditPhone, &QLineEdit::textChanged, this, &GroupDialogYW::onInputChanged);
    connect(m_lineEditBirthday, &QLineEdit::textChanged, this, &GroupDialogYW::onInputChanged);
    connect(m_lineEditOccupation, &QLineEdit::textChanged, this, &GroupDialogYW::onInputChanged);
    connect(m_lineEditAddress, &QLineEdit::textChanged, this, &GroupDialogYW::onInputChanged);
    connect(m_lineEditCity, &QLineEdit::textChanged, this, &GroupDialogYW::onInputChanged);
    connect(m_lineEditZipCode, &QLineEdit::textChanged, this, &GroupDialogYW::onInputChanged);

    // 连接按钮信号
    connect(m_startStopBtn, &QPushButton::clicked, this, &GroupDialogYW::onStartStopClicked);

    // 连接定时器信号
    connect(m_timer, &QTimer::timeout, this, &GroupDialogYW::onTimerTimeout);

    // 连接"写入"按钮信号
    connect(m_writeButton, &QPushButton::clicked, this, &GroupDialogYW::onWriteButtonClicked);
}

// ============================================
// 数据生成方法
// ============================================

void GroupDialogYW::generateInitialData()
{
    m_randomData.clear();
    m_constData.clear();

    // 生成初始的随机数据（填充历史数据 -30 到 0 秒）
    QRandomGenerator* gen = QRandomGenerator::global();
    int initialPoints = m_maxDataPoints;

    // 从 -30 秒开始，到 0 秒结束
    for (int i = 0; i < initialPoints; ++i) {
        double x = -m_displayDuration + (static_cast<double>(i) / initialPoints) * m_displayDuration;
        double y = 0.2 + (1.5 - 0.2) * gen->generateDouble();
        m_randomData.append(QPointF(x, y));
    }

    // m_currentTime 从 0 开始
    m_currentTime = 0.0;

    // 生成恒定值数据（与随机数据相同的x范围）
    m_constData.clear();
    for (int i = 0; i < m_randomData.size(); ++i) {
        double x = m_randomData[i].x();
        m_constData.append(QPointF(x, CONST_VALUE));
    }
}

void GroupDialogYW::addRandomDataPoint()
{
    // 增加当前时间（每秒增加1）
    m_currentTime += 1.0;

    // 生成随机值（在0.2到1.5之间）
    QRandomGenerator* gen = QRandomGenerator::global();
    double y = 0.2 + (1.5 - 0.2) * gen->generateDouble();

    // 添加新数据点
    m_randomData.append(QPointF(m_currentTime, y));

    // 如果数据点超过最大数量，移除最早的点
    while (m_randomData.size() > m_maxDataPoints) {
        m_randomData.removeFirst();
    }

    // 更新恒定值数据（保持与随机数据相同的x范围）
    m_constData.clear();
    for (int i = 0; i < m_randomData.size(); ++i) {
        double x = m_randomData[i].x();
        m_constData.append(QPointF(x, CONST_VALUE));
    }
}

// ============================================
// 清除曲线（保留坐标系）
// ============================================

void GroupDialogYW::clearCurves()
{
    // 移除所有曲线相关元素，保留坐标系
    QList<QGraphicsItem*> items = m_graphicsScene->items();
    for (QGraphicsItem* item : items) {
        int type = item->type();
        // 移除路径（曲线）和椭圆（数据点）
        if (type == QGraphicsPathItem::Type ||
            type == QGraphicsEllipseItem::Type) {
            m_graphicsScene->removeItem(item);
            delete item;
        }
    }
}

// ============================================
// 坐标系绘制方法（只绘制一次）
// ============================================

void GroupDialogYW::drawCoordinateSystem()
{
    // 如果已经绘制过坐标系，不再重复绘制
    if (m_coordinateSystemDrawn) {
        return;
    }

    // 清除场景
    m_graphicsScene->clear();

    // 获取视图大小
    QRectF sceneRect = m_graphicsView->sceneRect();
    if (sceneRect.isEmpty()) {
        sceneRect = QRectF(0, 0, 600, 350);
    }

    qreal width = sceneRect.width();
    qreal height = sceneRect.height();

    qreal margin = 50;
    qreal axisLen = 25;

    qreal originX = margin;
    qreal originY = height - margin;
    qreal xAxisEnd = width - margin;
    qreal yAxisEnd = margin;

    QPen axisPen(Qt::black, 2);
    QPen arrowPen(Qt::black, 2);
    QBrush arrowBrush(Qt::black);

    // ---------- 绘制X轴 ----------
    m_graphicsScene->addLine(originX, originY, xAxisEnd, originY, axisPen);

    // X轴箭头
    QPolygonF xArrow;
    xArrow << QPointF(xAxisEnd, originY)
        << QPointF(xAxisEnd - axisLen, originY - axisLen / 2)
        << QPointF(xAxisEnd - axisLen, originY + axisLen / 2);
    m_graphicsScene->addPolygon(xArrow, arrowPen, arrowBrush);

    // X轴标签
    QGraphicsTextItem* xLabel = m_graphicsScene->addText("Time (s)");
    xLabel->setPos(xAxisEnd - 30, originY - 10);
    xLabel->setDefaultTextColor(Qt::black);
    QFont font = xLabel->font();
    font.setBold(true);
    font.setPointSize(10);
    xLabel->setFont(font);

    // ---------- 绘制Y轴 ----------
    m_graphicsScene->addLine(originX, originY, originX, yAxisEnd, axisPen);

    // Y轴箭头
    QPolygonF yArrow;
    yArrow << QPointF(originX, yAxisEnd)
        << QPointF(originX - axisLen / 2, yAxisEnd + axisLen)
        << QPointF(originX + axisLen / 2, yAxisEnd + axisLen);
    m_graphicsScene->addPolygon(yArrow, arrowPen, arrowBrush);

    // Y轴标签
    QGraphicsTextItem* yLabel = m_graphicsScene->addText("Value");
    yLabel->setPos(originX + 10, yAxisEnd - 20);
    yLabel->setDefaultTextColor(Qt::black);
    yLabel->setFont(font);

    // ---------- 绘制原点 ----------
    QGraphicsTextItem* originLabel = m_graphicsScene->addText("0");
    originLabel->setPos(originX - 15, originY + 5);
    originLabel->setDefaultTextColor(Qt::black);

    // ---------- 绘制网格线 ----------
    QPen gridPen(Qt::gray, 1, Qt::DashLine);
    gridPen.setStyle(Qt::DotLine);

    // Y轴刻度（0到2.0，步长0.5）
    qreal yMax = Y_MAX;
    qreal yMin = Y_MIN;
    qreal yStep = 0.5;
    qreal plotHeight = height - 2 * margin;

    for (qreal y = yMin + yStep; y <= yMax; y += yStep) {
        qreal yPos = originY - (y / yMax) * plotHeight;
        if (yPos > yAxisEnd && yPos < originY) {
            m_graphicsScene->addLine(originX, yPos, xAxisEnd, yPos, gridPen);

            QGraphicsTextItem* label = m_graphicsScene->addText(QString::number(y, 'f', 1));
            label->setPos(originX - 30, yPos - 8);
            label->setDefaultTextColor(Qt::gray);
            QFont smallFont;
            smallFont.setPointSize(8);
            label->setFont(smallFont);
        }
    }

    // X轴刻度 - 显示从0到displayDuration
    double xMin = 0.0;
    double xMax = m_displayDuration;
    double xRange = xMax - xMin;
    qreal plotWidth = width - 2 * margin;
    int step = 5; // 每5秒一个刻度

    for (double x = 0; x <= xMax; x += step) {
        qreal xPos = originX + ((x - xMin) / xRange) * plotWidth;
        if (xPos > originX && xPos < xAxisEnd) {
            m_graphicsScene->addLine(xPos, originY, xPos, originY + 5, axisPen);

            QGraphicsTextItem* label = m_graphicsScene->addText(QString::number(static_cast<int>(x)));
            label->setPos(xPos - 8, originY + 8);
            label->setDefaultTextColor(Qt::gray);
            QFont smallFont;
            smallFont.setPointSize(8);
            label->setFont(smallFont);
        }
    }

    // 标记坐标系已绘制
    m_coordinateSystemDrawn = true;
}

// ============================================
// 曲线绘制方法（只绘制曲线，不清除坐标系）
// ============================================

void GroupDialogYW::drawCurves()
{
    // 先清除旧的曲线
    clearCurves();

    // 如果坐标系还没有绘制，先绘制坐标系
    if (!m_coordinateSystemDrawn) {
        drawCoordinateSystem();
    }

    // 获取视图大小
    QRectF sceneRect = m_graphicsView->sceneRect();
    if (sceneRect.isEmpty()) {
        sceneRect = QRectF(0, 0, 600, 350);
    }

    qreal width = sceneRect.width();
    qreal height = sceneRect.height();
    qreal margin = 50;

    qreal originX = margin;
    qreal originY = height - margin;
    qreal plotWidth = width - 2 * margin;
    qreal plotHeight = height - 2 * margin;

    // Y轴范围
    qreal yMax = Y_MAX;
    qreal yMin = Y_MIN;

    // X轴范围 - 固定显示 0 到 displayDuration
    double xMin = 0.0;
    double xMax = m_displayDuration;
    double xRange = xMax - xMin;
    if (xRange < 1.0) xRange = 1.0;

    QFont legendFont;
    legendFont.setPointSize(9);

    // ---------- 绘制恒定值曲线 ----------
    if (!m_constData.isEmpty()) {
        QPainterPath constPath;
        bool first = true;
        bool hasVisiblePoint = false;

        for (const QPointF& p : m_constData) {
            // 只绘制在显示范围内的点 (x >= 0 且 x <= displayDuration)
            if (p.x() < xMin || p.x() > xMax) continue;

            qreal x = originX + ((p.x() - xMin) / xRange) * plotWidth;
            qreal y = originY - ((p.y() - yMin) / (yMax - yMin)) * plotHeight;

            if (first) {
                constPath.moveTo(x, y);
                first = false;
                hasVisiblePoint = true;
            }
            else {
                constPath.lineTo(x, y);
            }
        }

        if (hasVisiblePoint) {
            QPen constPen(Qt::blue, 2);
            constPen.setStyle(Qt::DashLine);
            QGraphicsPathItem* pathItem = m_graphicsScene->addPath(constPath, constPen);
            pathItem->setZValue(1);
        }

        // 图例 - 恒定值
        QGraphicsTextItem* legend1 = m_graphicsScene->addText("Constant (y=0.8)");
        legend1->setPos(originX + 10, margin + 10);
        legend1->setDefaultTextColor(Qt::blue);
        legend1->setFont(legendFont);
        legend1->setZValue(1);
    }

    // ---------- 绘制随机值曲线 ----------
    if (!m_randomData.isEmpty()) {
        QPainterPath randomPath;
        bool first = true;
        bool hasVisiblePoint = false;

        // 只绘制 x >= 0 的数据点
        for (const QPointF& p : m_randomData) {
            if (p.x() < xMin || p.x() > xMax) continue;

            qreal x = originX + ((p.x() - xMin) / xRange) * plotWidth;
            qreal y = originY - ((p.y() - yMin) / (yMax - yMin)) * plotHeight;

            if (first) {
                randomPath.moveTo(x, y);
                first = false;
                hasVisiblePoint = true;
            }
            else {
                randomPath.lineTo(x, y);
            }
        }

        if (hasVisiblePoint) {
            QPen randomPen(Qt::red, 2);
            randomPen.setStyle(Qt::SolidLine);
            QGraphicsPathItem* pathItem = m_graphicsScene->addPath(randomPath, randomPen);
            pathItem->setZValue(1);

            // 绘制数据点
            QBrush pointBrush(Qt::red);
            QPen pointPen(Qt::red, 1);
            for (const QPointF& p : m_randomData) {
                if (p.x() < xMin || p.x() > xMax) continue;

                qreal x = originX + ((p.x() - xMin) / xRange) * plotWidth;
                qreal y = originY - ((p.y() - yMin) / (yMax - yMin)) * plotHeight;
                QGraphicsEllipseItem* ellipse = m_graphicsScene->addEllipse(x - 2, y - 2, 4, 4, pointPen, pointBrush);
                ellipse->setZValue(2);
            }
        }

        // 图例 - 随机值
        QGraphicsTextItem* legend2 = m_graphicsScene->addText("Random Value");
        legend2->setPos(originX + 10, margin + 28);
        legend2->setDefaultTextColor(Qt::red);
        legend2->setFont(legendFont);
        legend2->setZValue(1);

        // 显示当前值（最新数据点）
        if (!m_randomData.isEmpty()) {
            QPointF lastPoint = m_randomData.last();
            QGraphicsTextItem* valueLabel = m_graphicsScene->addText(
                QString("Current: %1").arg(lastPoint.y(), 0, 'f', 3)
            );
            valueLabel->setPos(width - 150, margin + 10);
            valueLabel->setDefaultTextColor(Qt::darkGray);
            QFont valueFont;
            valueFont.setPointSize(9);
            valueFont.setBold(true);
            valueLabel->setFont(valueFont);
            valueLabel->setZValue(1);
        }
    }
}

// ============================================
// Start/Stop 按钮
// ============================================

void GroupDialogYW::onStartStopClicked()
{
    if (m_isRunning) {
        // 停止
        m_timer->stop();
        m_isRunning = false;
        m_startStopBtn->setText("开始");
        m_startStopBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; }");
    }
    else {
        // 启动 - 直接开始，不重新生成数据
        m_timer->start();
        m_isRunning = true;
        m_startStopBtn->setText("停止");
        m_startStopBtn->setStyleSheet("QPushButton { background-color: #f44336; color: white; font-weight: bold; }");
    }
}

void GroupDialogYW::onTimerTimeout()
{
    // 每秒添加一个新的随机数据点
    addRandomDataPoint();

    // 只重绘曲线，不重绘坐标系
    drawCurves();
}

// ============================================
// 按钮状态更新
// ============================================

void GroupDialogYW::onInputChanged()
{
    // 废除所有字段必须填写的限制，"读取"按钮始终可用
    // 获取"读取"按钮并启用
    QList<QAbstractButton*> buttons = m_buttonBox->buttons();
    for (QAbstractButton* btn : buttons) {
        if (btn->text() == "读取") {
            btn->setEnabled(true);
            break;
        }
    }
}

// ============================================
// "写入" 按钮槽函数
// ============================================

void GroupDialogYW::onWriteButtonClicked()
{
    // 获取所有输入的数据
    QString name = getName();
    QString email = getEmail();
    QString phone = getPhone();
    QString birthday = getBirthday();
    QString occupation = getOccupation();
    QString address = getAddress();
    QString city = getCity();
    QString zipCode = getZipCode();

    // 检查是否所有字段都已填写
    if (name.isEmpty() || email.isEmpty() || phone.isEmpty() ||
        birthday.isEmpty() || occupation.isEmpty() ||
        address.isEmpty() || city.isEmpty() || zipCode.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please fill in all fields before writing.");
        return;
    }

    // 显示写入确认消息
    QMessageBox::information(this, "Write Data",
        QString("Data written successfully!\n\n"
            "Personal Information:\n"
            "  Name       : %1\n"
            "  Email      : %2\n"
            "  Phone      : %3\n"
            "  Birthday   : %4\n"
            "  Occupation : %5\n\n"
            "Address Information:\n"
            "  Address    : %6\n"
            "  City       : %7\n"
            "  Zip Code   : %8")
        .arg(name)
        .arg(email)
        .arg(phone)
        .arg(birthday)
        .arg(occupation)
        .arg(address)
        .arg(city)
        .arg(zipCode));
}

// ============================================
// 个人信息 Getter/Setter
// ============================================

QString GroupDialogYW::getName() const
{
    return m_lineEditName->text().trimmed();
}

QString GroupDialogYW::getEmail() const
{
    return m_lineEditEmail->text().trimmed();
}

QString GroupDialogYW::getPhone() const
{
    return m_lineEditPhone->text().trimmed();
}

QString GroupDialogYW::getBirthday() const
{
    return m_lineEditBirthday->text().trimmed();
}

QString GroupDialogYW::getOccupation() const
{
    return m_lineEditOccupation->text().trimmed();
}

void GroupDialogYW::setName(const QString& name)
{
    m_lineEditName->setText(name);
}

void GroupDialogYW::setEmail(const QString& email)
{
    m_lineEditEmail->setText(email);
}

void GroupDialogYW::setPhone(const QString& phone)
{
    m_lineEditPhone->setText(phone);
}

void GroupDialogYW::setBirthday(const QString& birthday)
{
    m_lineEditBirthday->setText(birthday);
}

void GroupDialogYW::setOccupation(const QString& occupation)
{
    m_lineEditOccupation->setText(occupation);
}

// ============================================
// 地址信息 Getter/Setter
// ============================================

QString GroupDialogYW::getAddress() const
{
    return m_lineEditAddress->text().trimmed();
}

QString GroupDialogYW::getCity() const
{
    return m_lineEditCity->text().trimmed();
}

QString GroupDialogYW::getZipCode() const
{
    return m_lineEditZipCode->text().trimmed();
}

void GroupDialogYW::setAddress(const QString& address)
{
    m_lineEditAddress->setText(address);
}

void GroupDialogYW::setCity(const QString& city)
{
    m_lineEditCity->setText(city);
}

void GroupDialogYW::setZipCode(const QString& zipCode)
{
    m_lineEditZipCode->setText(zipCode);
}

void GroupDialogYW::clearInputs()
{
    m_lineEditName->clear();
    m_lineEditEmail->clear();
    m_lineEditPhone->clear();
    m_lineEditBirthday->clear();
    m_lineEditOccupation->clear();
    m_lineEditAddress->clear();
    m_lineEditCity->clear();
    m_lineEditZipCode->clear();
}