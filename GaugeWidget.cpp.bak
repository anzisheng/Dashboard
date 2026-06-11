#include "GaugeWidget.h"
#include <QRadialGradient>
#include <QFontMetrics>

GaugeWidget::GaugeWidget(QWidget* parent)
    : QWidget(parent)
    , m_minValue(0.0)
    , m_maxValue(100.0)
    , m_value(0.0)
    , m_unit("")
    , m_radius(0)
    , m_backgroundColor(30, 30, 35)
    , m_gaugeColor(70, 130, 200)
    , m_needleColor(255, 80, 80)
    , m_textColor(220, 220, 220)
    , m_majorTickCount(10)
    , m_minorTickCount(4)
{
    setMinimumSize(100, 100);
    setBackgroundRole(QPalette::Window);
    setAutoFillBackground(true);
}

GaugeWidget::~GaugeWidget() = default;

void GaugeWidget::setMinValue(double min)
{
    if (min >= m_maxValue) return;
    m_minValue = min;
    update();
}

void GaugeWidget::setMaxValue(double max)
{
    if (max <= m_minValue) return;
    m_maxValue = max;
    if (m_value > m_maxValue) m_value = m_maxValue;
    if (m_value < m_minValue) m_value = m_minValue;
    update();
}

void GaugeWidget::setRange(double min, double max)
{
    if (min >= max) return;
    m_minValue = min;
    m_maxValue = max;
    if (m_value > m_maxValue) m_value = m_maxValue;
    if (m_value < m_minValue) m_value = m_minValue;
    update();
}

void GaugeWidget::setValue(double value)
{
    double newVal = qBound(m_minValue, value, m_maxValue);
    if (qFuzzyCompare(m_value, newVal)) return;
    m_value = newVal;
    update();
}

void GaugeWidget::setUnit(const QString& unit)
{
    m_unit = unit;
    update();
}

void GaugeWidget::setGaugeColor(const QColor& color)
{
    m_gaugeColor = color;
    update();
}

void GaugeWidget::setNeedleColor(const QColor& color)
{
    m_needleColor = color;
    update();
}

void GaugeWidget::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    int side = qMin(width(), height());
    m_radius = side * 9 / 20;
    m_center = QPointF(width() / 2.0, height() * 0.6);
}

void GaugeWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    drawBackground(&painter);
    drawScale(&painter);
    drawScaleNumbers(&painter);
    drawNeedle(&painter);
    drawCenterCircle(&painter);
    drawValueDisplay(&painter);
}

void GaugeWidget::drawBackground(QPainter* painter)
{
    QRadialGradient gradient(m_center, m_radius);
    gradient.setColorAt(0, QColor(50, 50, 55));
    gradient.setColorAt(1, QColor(20, 20, 25));

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(gradient);
    painter->drawEllipse(m_center, m_radius, m_radius);
    painter->restore();
}

double GaugeWidget::valueToAngle(double value) const
{
    double ratio = (value - m_minValue) / (m_maxValue - m_minValue);
    return START_ANGLE + ratio * SPAN_ANGLE;
}

void GaugeWidget::drawScale(QPainter* painter)
{
    painter->save();
    painter->translate(m_center);
    painter->rotate(START_ANGLE - 90);

    double angleStep = SPAN_ANGLE / m_majorTickCount;
    double minorStep = angleStep / (m_minorTickCount + 1);

    QPen majorPen(m_textColor);
    majorPen.setWidthF(3);
    QPen minorPen(m_textColor);
    minorPen.setWidthF(1.5);

    int total = m_majorTickCount * (m_minorTickCount + 1);

    for (int i = 0; i <= total; ++i)
    {
        double angle = i * minorStep;
        if (angle > SPAN_ANGLE + 0.1) break;

        painter->save();
        painter->rotate(angle);

        if (i % (m_minorTickCount + 1) == 0)
        {
            painter->setPen(majorPen);
            painter->drawLine(m_radius - 8, 0, m_radius - 2, 0);
        }
        else
        {
            painter->setPen(minorPen);
            painter->drawLine(m_radius - 5, 0, m_radius - 2, 0);
        }

        painter->restore();
    }
    painter->restore();
}

void GaugeWidget::drawScaleNumbers(QPainter* painter)
{
    painter->save();
    painter->setPen(m_textColor);
    painter->setFont(QFont("Arial", m_radius / 12, QFont::Bold));

    double angleStep = SPAN_ANGLE / m_majorTickCount;

    for (int i = 0; i <= m_majorTickCount; ++i)
    {
        double angle = START_ANGLE + i * angleStep;
        double rad = qDegreesToRadians(angle);

        int tickValue = m_minValue + (m_maxValue - m_minValue) * i / m_majorTickCount;
        QString label = QString::number(tickValue);

        int labelRadius = m_radius - 18;
        double x = m_center.x() + labelRadius * qCos(rad);
        double y = m_center.y() - labelRadius * qSin(rad);

        QRectF rect(x - 12, y - 8, 24, 16);
        painter->drawText(rect, Qt::AlignCenter, label);
    }
    painter->restore();
}

void GaugeWidget::drawNeedle(QPainter* painter)
{
    painter->save();
    painter->translate(m_center);
    painter->rotate(valueToAngle(m_value) - 90);

    int needleLength = m_radius - 18;
    int needleWidthBase = 6;

    QPolygon needlePoly;
    needlePoly << QPoint(needleLength, 0)
        << QPoint(-needleWidthBase, needleWidthBase / 2)
        << QPoint(-needleWidthBase / 2, 0)
        << QPoint(-needleWidthBase, -needleWidthBase / 2);

    painter->setPen(Qt::NoPen);
    painter->setBrush(m_needleColor);
    painter->drawPolygon(needlePoly);
    painter->restore();
}

void GaugeWidget::drawCenterCircle(QPainter* painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    QRadialGradient gradient(m_center, 12);
    gradient.setColorAt(0, QColor(220, 220, 230));
    gradient.setColorAt(1, QColor(100, 100, 110));
    painter->setBrush(gradient);
    painter->drawEllipse(m_center, 10, 10);
    painter->restore();
}

void GaugeWidget::drawValueDisplay(QPainter* painter)
{
    QString text = QString::number(m_value, 'f', 1);
    if (!m_unit.isEmpty())
        text += " " + m_unit;

    painter->setPen(m_textColor);
    QFont font("Arial", m_radius / 8, QFont::Bold);
    painter->setFont(font);

    QRectF textRect(m_center.x() - 60, m_center.y() + 25, 120, 35);
    painter->drawText(textRect, Qt::AlignCenter, text);
}