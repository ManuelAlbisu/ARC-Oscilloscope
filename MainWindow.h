#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>

#include <QSpinBox>
#include <QSlider>
#include <QDial>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void update();
    void setPeriod(int period);
    void setAmplitude(int amplitude);
    void setPhase(int phase);
    void periodControl();
    void amplitudeControl();
    void phaseControl();

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
    QSlider *m_perSlider;
    QLabel *m_perLabel;

    QSpinBox *m_ampSpinBox;
    QSlider *m_ampSlider;
    QLabel *m_ampLabel;

    QSpinBox *m_phaseSpinBox;
    QDial *m_phaseDial;
    QLabel *m_phaseLabel;
};

#endif // MAINWINDOW_H
