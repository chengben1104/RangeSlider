#ifndef RANGESLIDER_H
#define RANGESLIDER_H

#include <QWidget>

class RangeSlider : public QWidget
{
    Q_OBJECT
public:
    explicit RangeSlider(Qt::Orientation orientation = Qt::Horizontal, QWidget* parent = nullptr);

    void setRange(int min, int max);
    void setValues(int lower, int upper);

    int lowerValue() const;
    int upperValue() const;
    int stepPixelLength() const;
    int stepPixelSpacing() const;

    void setOrientation(Qt::Orientation orientation);
    void setShowHighlight(bool show);
    Qt::Orientation orientation() const;

signals:
    void rangeChanged(int lower, int upper);
    void rangeChangedRelease(int lower, int upper);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

private:
    int m_min = 0;
    int m_max = 100;
    int m_lower = 0;
    int m_upper = 100;
    int m_oldLower = 0;
    int m_oldUpper = 100;
    int m_handleRadius = 13;
    bool m_draggingLower = false;
    bool m_draggingUpper = false;
    bool m_showHighlight = false;
    Qt::Orientation m_orientation = Qt::Horizontal;

    QRect lowerHandleRect() const;
    QRect upperHandleRect() const;

    int valueToPos(int value) const;
    int posToValue(int pos) const;
};

#endif // RANGESLIDER_H
