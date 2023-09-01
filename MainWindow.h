#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>

#include <QSpinBox>
#include <QSlider>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void update();
    void setPeriod(int p);
    void setAmplitude(int a);
    void periodControl();
    void amplitudeControl();

private:
    // Handles the visual representation of the graph
    QChartView *m_graphView;

    // Handles the functions of the graph
    QLineSeries *m_sine;
    QLineSeries *m_cosine;

    // y(t) = amplitude * sin((2 * PI / period) * t + phase);
    qreal m_amplitude;
    qreal m_period;
    qreal m_phase;
    qreal m_time;
    qreal m_deltaTime;

    QValueAxis *m_xAxis;
    QValueAxis *m_yAxis;

    QSpinBox *m_perSpinBox;
    QSpinBox *m_ampSpinBox;
    QSlider *m_perSlider;
    QSlider *m_ampSlider;
};

#endif // MAINWINDOW_H
