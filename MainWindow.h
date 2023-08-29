#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SlidersGroup.h"

#include <QMainWindow>
#include <QChartView>
#include <QChart>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineSeries>
#include <QSlider>

#include <QStackedWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void update();

private:
    // Handles the visual representation of the graph
    QChart *m_graphScene;
    QChartView *m_graphView;

    // Handles the layout of the widgets
    QHBoxLayout *m_mainLayout;
    QVBoxLayout *m_sliderLayout;
    QStackedWidget *m_stackedWidget;

    // Handles the sliders
    SlidersGroup *m_horizontalSliders;
    SlidersGroup *m_verticalSliders;

    // Handles the functions of the graph
    QLineSeries *m_sine;
    QLineSeries *m_cosine;

    // y(t) = amplitude * sin((2 * PI / period) * t + phase);
    qreal m_amplitude;
    qreal m_period;
    qreal m_phase;
    qreal m_time;
    qreal m_deltaTime;
};

#endif // MAINWINDOW_H
