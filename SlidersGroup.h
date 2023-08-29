#ifndef SLIDERSGROUP_H
#define SLIDERSGROUP_H

#include <QGroupBox>
#include <QBoxLayout>
#include <QSlider>
#include <QScrollBar>
#include <QDial>

class SlidersGroup : public QGroupBox
{
    Q_OBJECT

public:
    SlidersGroup(Qt::Orientation orientation, const QString &title,
          QWidget *parent = nullptr);

signals:
    void valueChanged(int value);

public slots:
    void setValue(int value);
    void setMinimum(int value);
    void setMaximum(int value);
    void invertAppearance(bool invert);
    void invertKeyBindings(bool invert);

private:
    QSlider *m_slider;
    QScrollBar *m_scrollBar;
    QDial *m_dial;
};

#endif // SLIDERSGROUP_H
