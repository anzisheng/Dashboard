#ifndef GAUGEWIDGET_H
#define GAUGEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QtMath>

class GaugeWidget : public QWidget
{
    Q_OBJECT
        Q_PROPERTY(double minValue READ minValue WRITE setMinValue)
        Q_PROPERTY(double maxValue READ maxValue WRITE setMaxValue)
        Q_PROPERTY(double value READ value WRITE setValue)
        Q_PROPERTY(QString unit READ unit WRITE setUnit)

public:
    explicit GaugeWidget(QWidget* parent = nullptr);
    ~GaugeWidget();

    double minValue() const { return m_minValue; }
    double maxValue() const { return m_maxValue; }
    void setMinValue(double min);
    void setMaxValue(double max);
    void setRange(double min, double max);

    double value() const { return m_value; }
    void setValue(double value);

    QString unit() const { return m_unit; }
    void setUnit(const QString& unit);

    void setGaugeColor(const QColor& color);
    void setNeedleColor(const QColor& color);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void drawBackground(QPainter* painter);
    void drawScale(QPainter* painter);
    void drawScaleNumbers(QPainter* painter);
    void drawNeedle(QPainter* painter);
    void drawCenterCircle(QPainter* painter);
    void drawValueDisplay(QPainter* painter);

    double valueToAngle(double value) const;

private:
    double m_minValue;
    double m_maxValue;
    double m_value;
    QString m_unit;

    int m_radius;
    QPointF m_center;

    static constexpr double START_ANGLE = 225.0;
    static constexpr double SPAN_ANGLE = 270.0;

    QColor m_backgroundColor;
    QColor m_gaugeColor;
    QColor m_needleColor;
    QColor m_textColor;

    int m_majorTickCount;
    int m_minorTickCount;
};

#endif // GAUGEWIDGET_H