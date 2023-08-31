#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QChart>
#include <QLineSeries>

// Testing
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSlider>
#include <QSpinBox>
#include <QDockWidget>

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
    QVBoxLayout *m_mainLayout;

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
