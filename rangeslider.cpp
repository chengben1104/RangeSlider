#include "RangeSlider.h"
#include <algorithm>
#include <QPainter>
#include <QMouseEvent>

RangeSlider::RangeSlider(Qt::Orientation orientation, QWidget* parent)
    : QWidget(parent), m_orientation(orientation)
{
    setMouseTracking(true);
    setMinimumSize(40, 40);
}

void RangeSlider::setRange(int min, int max)
{
    if(min == max){
        return;
    }

    m_min = std::min(min, max);
    m_max = std::max(min, max);
    m_lower = m_min;
    m_upper = m_max;
    update();
}

void RangeSlider::setValues(int lower, int upper)
{
    if(lower == upper){
        return;
    }

    m_lower = std::min(lower, upper);
    m_upper = std::max(lower, upper);
    update();
}

int RangeSlider::lowerValue() const
{
    return m_lower;
}

int RangeSlider::upperValue() const
{
    return m_upper;
}

int RangeSlider::stepPixelLength() const
{
    int pos = valueToPos(0);
    if (m_orientation == Qt::Horizontal) {
        int x = pos - m_handleRadius;
        int y = height() / 2 - m_handleRadius;
        return QRect(x, y, 2 * m_handleRadius, 2 * m_handleRadius).normalized().width();
    } else {
        int y = pos - m_handleRadius;
        int x = width() / 2 - m_handleRadius;
        return QRect(x, y, 2 * m_handleRadius, 2 * m_handleRadius).normalized().height();
    }
}

int RangeSlider::stepPixelSpacing() const
{
    int pos1 = valueToPos(0);
    int pos2 = valueToPos(1);

    return (pos2 - pos1 - 2 * m_handleRadius);
}

void RangeSlider::setOrientation(Qt::Orientation orientation)
{
    if (m_orientation != orientation) {
        m_orientation = orientation;
        update();
    }
}

void RangeSlider::setShowHighlight(bool show)
{
    if (m_showHighlight != show) {
        m_showHighlight = show;
        update();
    }
}

Qt::Orientation RangeSlider::orientation() const
{
    return m_orientation;
}

void RangeSlider::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    if (m_orientation == Qt::Horizontal) {
        int barY = height() / 2 - 2;
        int barHeight = 4;

        int lowerX = valueToPos(m_lower);
        int upperX = valueToPos(m_upper);

        p.setBrush(QColor(0xbdbebf));
        p.drawRect(m_handleRadius, barY, width() - 2 * m_handleRadius, barHeight);

        p.setBrush(m_showHighlight ? QColor(0x21be2b) : QColor(0xbdbebf));
        p.drawRect(lowerX, barY, upperX - lowerX, barHeight);

        p.setBrush(QColor(0x61c6f6));
        p.setPen(QPen(Qt::white, 1));
        p.drawEllipse(lowerHandleRect());
        p.drawEllipse(upperHandleRect());
    } else {
        int barX = width() / 2 - 2;
        int barWidth = 4;

        int lowerY = valueToPos(m_lower);
        int upperY = valueToPos(m_upper);

        p.setBrush(QColor(0xbdbebf));
        p.drawRect(barX, m_handleRadius, barWidth, height() - 2 * m_handleRadius);

        p.setBrush(m_showHighlight ? QColor(0x21be2b) : QColor(0xbdbebf));
        p.drawRect(barX, lowerY, barWidth, upperY - lowerY);

        p.setBrush(QColor(0x61c6f6));
        p.setPen(QPen(Qt::white, 1));
        p.drawEllipse(lowerHandleRect());
        p.drawEllipse(upperHandleRect());
    }
}

void RangeSlider::mousePressEvent(QMouseEvent* event)
{
    if (lowerHandleRect().contains(event->pos())) {
        m_draggingLower = true;
        m_oldLower = m_lower;
    } else if (upperHandleRect().contains(event->pos())) {
        m_draggingUpper = true;
        m_oldUpper = m_upper;
    }
}

void RangeSlider::mouseMoveEvent(QMouseEvent* event)
{
    int pos = (m_orientation == Qt::Horizontal) ? event->pos().x() : event->pos().y();
    if (m_draggingLower) {
        int val = posToValue(pos);
        if (val < m_upper && val != m_lower) {
            m_lower = val;
            emit rangeChanged(m_lower, m_upper);
            update();
        }
    } else if (m_draggingUpper) {
        int val = posToValue(pos);
        if (val > m_lower && val != m_upper) {
            m_upper = val;
            emit rangeChanged(m_lower, m_upper);
            update();
        }
    }
}

void RangeSlider::mouseReleaseEvent(QMouseEvent*)
{
    if (m_oldLower != m_lower || m_oldUpper != m_upper){
        m_oldLower = m_lower;
        m_oldUpper = m_upper;
        emit rangeChangedRelease(m_lower, m_upper);
        update();
    }

    m_draggingLower = m_draggingUpper = false;
}

QRect RangeSlider::lowerHandleRect() const
{
    if (m_orientation == Qt::Horizontal) {
        int x = valueToPos(m_lower) - m_handleRadius;
        int y = height() / 2 - m_handleRadius;
        return QRect(x, y, 2 * m_handleRadius, 2 * m_handleRadius);
    } else {
        int y = valueToPos(m_lower) - m_handleRadius;
        int x = width() / 2 - m_handleRadius;
        return QRect(x, y, 2 * m_handleRadius, 2 * m_handleRadius);
    }
}

QRect RangeSlider::upperHandleRect() const
{
    if (m_orientation == Qt::Horizontal) {
        int x = valueToPos(m_upper) - m_handleRadius;
        int y = height() / 2 - m_handleRadius;
        return QRect(x, y, 2 * m_handleRadius, 2 * m_handleRadius);
    } else {
        int y = valueToPos(m_upper) - m_handleRadius;
        int x = width() / 2 - m_handleRadius;
        return QRect(x, y, 2 * m_handleRadius, 2 * m_handleRadius);
    }
}

int RangeSlider::valueToPos(int value) const
{
    double ratio = double(value - m_min) / (m_max - m_min);
    int length = (m_orientation == Qt::Horizontal ? width() : height()) - 2 * m_handleRadius;
    return m_handleRadius + int(ratio * length);
}

int RangeSlider::posToValue(int pos) const
{
    int span = (m_orientation == Qt::Horizontal ? width() : height()) - 2 * m_handleRadius;
    int coord = std::clamp(pos - m_handleRadius, 0, span);
    double ratio = double(coord) / span;
    return m_min + int(ratio * (m_max - m_min));
}
