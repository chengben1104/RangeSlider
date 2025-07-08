#include "sliderlistwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

SliderListWidget::SliderListWidget(
    const QStringList &labels,
    const QSize& size,
    Qt::Orientation orientation,
    QWidget *parent)
    : QWidget{parent}
{
    auto labelList = new QWidget(this);
    m_slider = new RangeSlider(orientation, this);
    m_slider->setRange(0, labels.size() - 1);
    m_slider->setFixedSize(size);

    (orientation == Qt::Vertical) ?
        inintVerLayout(labelList, labels) :
        inintHorLayout(labelList, labels);

    traverseLabelMap(0, labels.size() - 1);

    connect(m_slider, &RangeSlider::rangeChanged, this, [this](int min, int max) {
        traverseLabelMap(min, max);
    });

    connect(m_slider, &RangeSlider::rangeChangedRelease, this, [this](int min, int max) {
        qDebug() << "Selected range:" << min << "to" << max;
        traverseLabelMap(min, max);
        emit this->rangeChanged(min, max);
    });
}

int SliderListWidget::lowerValue() const
{
    return m_slider->lowerValue();
}

int SliderListWidget::upperValue() const
{
    return m_slider->upperValue();
}

void SliderListWidget::inintHorLayout(QWidget *wgt, const QStringList &labels)
{
    m_labelsMap.clear();
    wgt->setFixedWidth(m_slider->width());

    auto* mainLayout = new QVBoxLayout;
    auto* hLayout = new QHBoxLayout;
    hLayout->setSpacing(m_slider->stepPixelSpacing() + 1); // offset
    hLayout->setContentsMargins(0, 0, 0, 0);

    int stepLength = m_slider->stepPixelLength();
    for (int i = 0; i < labels.size(); ++i) {
        auto* label = new QLabel(labels[i], this);
        label->setFixedWidth(stepLength);
        label->setFixedHeight(80);
        hLayout->addWidget(label);
        m_labelsMap[i] = label;
    }
    wgt->setLayout(hLayout);
    hLayout->addStretch();

    mainLayout->addWidget(m_slider);
    mainLayout->addWidget(wgt);

    this->setLayout(mainLayout);
}

void SliderListWidget::inintVerLayout(QWidget* wgt, const QStringList &labels)
{
    m_labelsMap.clear();
    wgt->setFixedHeight(m_slider->height());

    auto* mainLayout = new QHBoxLayout;
    auto* vLayout = new QVBoxLayout;
    vLayout->setSpacing(m_slider->stepPixelSpacing() + 1); // offset
    vLayout->setContentsMargins(0, 0, 0, 0);

    int stepLength = m_slider->stepPixelLength();
    for (int i = 0; i < labels.size(); ++i) {
        auto* label = new QLabel(labels[i], this);
        label->setFixedHeight(stepLength);
        label->setFixedWidth(80);
        vLayout->addWidget(label);
        m_labelsMap[i] = label;
    }
    wgt->setLayout(vLayout);
    vLayout->addStretch();

    mainLayout->addWidget(m_slider);
    mainLayout->addWidget(wgt);

    this->setLayout(mainLayout);
}

void SliderListWidget::traverseLabelMap(int val1, int val2)
{
    for(const auto& key : m_labelsMap.keys()){
        if(key == val1 || key == val2){
            m_labelsMap[key]->setStyleSheet("color:#61c6f6");
        }
        else{
            m_labelsMap[key]->setStyleSheet("color:black");
        }
    }
}
