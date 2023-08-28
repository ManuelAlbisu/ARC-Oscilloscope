#include "MainWindow.h"

#include <QtMath>
#include <QValueAxis>
#include <QTimer>
//#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Set variables
    m_amplitude = 2.0;
    m_period = 5.0;
    m_phase = 0.0;

    m_time = 0.0;
    m_deltaTime = 0.01;

    m_sine = new QLineSeries();
    m_cosine = new QLineSeries();

    // Creates x-axis for time in seconds
    auto xAxis = new QValueAxis;
    xAxis->setRange(0, m_period);
    xAxis->setTitleText("Time (s");

    // Creates y-axis for voltage
    auto yAxis = new QValueAxis;
    yAxis->setRange(-m_amplitude, m_amplitude);
    yAxis->setTitleText("Voltage (V)");

    // Assigns position of x & y axis to the graph
    m_view = new QChartView;
    auto chart = m_view->chart();
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);

    // Adds functions to the graph
    chart->addSeries(m_sine);
    chart->addSeries(m_cosine);

    // Assigns x & y axis to the sine/cosine function of the graph
    m_sine->attachAxis(xAxis);
    m_sine->attachAxis(yAxis);

    m_cosine->attachAxis(xAxis);
    m_cosine->attachAxis(yAxis);

    // Sets the graphs title
    chart->legend()->hide();
    chart->setTitle("ARC Oscilloscope");

    // Sets the graph as the applications central widget
    setCentralWidget(m_view);
    resize(1000, 800);

    // Sets a timer to update the graphs contents in milliseconds
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
    timer->start(1000 * m_deltaTime);
}

MainWindow::~MainWindow()
{
    // Destroys the main window
}

void MainWindow::update()
{
    m_time += m_deltaTime;

    // Restarts plotting if graph reaches end of range
    if (m_time > m_period) {
        m_time = 0.0;
        m_sine->clear();
        m_cosine->clear();
    }

    // Gets the voltage of the sine/cosine functions
    qreal y1 = m_amplitude * sin((2 * M_PI / m_period) * m_time + m_phase);
    qreal y2 = m_amplitude * cos((2 * M_PI / m_period) * m_time + m_phase);

    // Plots time and voltage to the graph
    m_sine->append(m_time, y1);
    m_cosine->append(m_time, y2);
}
