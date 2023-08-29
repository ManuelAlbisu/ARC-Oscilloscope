#include "SlidersGroup.h"

SlidersGroup::SlidersGroup(Qt::Orientation orientation, const QString &title,
                           QWidget *parent)
    : QGroupBox(title, parent) {
    m_slider = new QSlider(orientation);
    m_slider->setFocusPolicy(Qt::StrongFocus);
    m_slider->setTickPosition(QSlider::TicksBothSides);
    m_slider->setTickInterval(10);
    m_slider->setSingleStep(1);

    m_scrollBar = new QScrollBar(orientation);
    m_scrollBar->setFocusPolicy(Qt::StrongFocus);

    m_dial = new QDial;
    m_dial->setFocusPolicy(Qt::StrongFocus);

    connect(m_slider, &QSlider::valueChanged, m_scrollBar, &QScrollBar::setValue);
    connect(m_scrollBar, &QScrollBar::valueChanged, m_dial, &QDial::setValue);
    connect(m_dial, &QDial::valueChanged, m_slider, &QSlider::setValue);

    connect(m_dial, &QDial::valueChanged, this, &SlidersGroup::valueChanged);

    QBoxLayout::Direction direction;

    if (orientation == Qt::Horizontal)
        direction = QBoxLayout::TopToBottom;
    else
        direction = QBoxLayout::LeftToRight;

    QBoxLayout *slidersLayout = new QBoxLayout(direction);
    slidersLayout->addWidget(m_slider);
    slidersLayout->addWidget(m_scrollBar);
    slidersLayout->addWidget(m_dial);
    setLayout(slidersLayout);
}

void SlidersGroup::setValue(int value) {
    m_slider->setValue(value);
}

void SlidersGroup::setMinimum(int value) {
    m_slider->setMinimum(value);
    m_scrollBar->setMinimum(value);
    m_dial->setMinimum(value);
}

void SlidersGroup::setMaximum(int value) {
    m_slider->setMaximum(value);
    m_scrollBar->setMaximum(value);
    m_dial->setMaximum(value);
}

void SlidersGroup::invertAppearance(bool invert) {
    m_slider->setInvertedAppearance(invert);
    m_scrollBar->setInvertedAppearance(invert);
    m_dial->setInvertedAppearance(invert);
}

void SlidersGroup::invertKeyBindings(bool invert) {
    m_slider->setInvertedControls(invert);
    m_scrollBar->setInvertedControls(invert);
    m_dial->setInvertedControls(invert);
}
