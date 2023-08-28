#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QChart>
#include <QLineSeries>

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
    QChart* m_chart;
    QChartView* m_view;

    // Handles the functions of the graph
    QLineSeries* m_sine;
    QLineSeries* m_cosine;

    // y(t) = amplitude * sin((2 * PI / period) * t + phase);
    qreal m_amplitude;
    qreal m_period;
    qreal m_phase;
    qreal m_time;
    qreal m_deltaTime;
};

#endif // MAINWINDOW_H
