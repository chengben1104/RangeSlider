#ifndef SLIDERLISTWIDGET_H
#define SLIDERLISTWIDGET_H

#include <QWidget>
#include <qlabel.h>
#include "rangeslider.h"

class SliderListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SliderListWidget(
        const QStringList& labels,
        const QSize& size,
        Qt::Orientation orientation = Qt::Horizontal,
        QWidget *parent = nullptr
    );

    int lowerValue() const;
    int upperValue() const;

signals:
    void rangeChanged(int lower, int upper);

protected:
    void traverseLabelMap(int val1, int val2);

private:
    void inintHorLayout(QWidget* wgt, const QStringList& labelNames);
    void inintVerLayout(QWidget* wgt, const QStringList& labelNames);

private:
    RangeSlider* m_slider{nullptr};
    QHash<int, QLabel*> m_labelsMap;
};

#endif // SLIDERLISTWIDGET_H
