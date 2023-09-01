#include "MainWindow.h"

#include <QtMath>
#include <QTimer>
#include <QDebug>
#include <QSlider>
#include <QSpinBox>

// Testing
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent)
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
    m_xAxis = new QValueAxis;
    m_xAxis->setTitleText("Time (s)");

    // Creates y-axis for voltage
    m_yAxis = new QValueAxis;
    m_yAxis->setTitleText("Voltage (V)");

    // Assigns position of x & y axis to the graph
    m_graphView = new QChartView;
    m_graphView->chart()->addAxis(m_xAxis, Qt::AlignBottom);
    m_graphView->chart()->addAxis(m_yAxis, Qt::AlignLeft);

    // Adds functions to the graph
    m_graphView->chart()->addSeries(m_sine);
    m_graphView->chart()->addSeries(m_cosine);

    // Assigns x & y axis to the sine/cosine function of the graph
    m_sine->attachAxis(m_xAxis);
    m_sine->attachAxis(m_yAxis);

    m_cosine->attachAxis(m_xAxis);
    m_cosine->attachAxis(m_yAxis);

    // Sets the graphs title
    m_graphView->chart()->legend()->hide();
    m_graphView->chart()->setTitle("ARC Oscilloscope");

    periodControl();
    amplitudeControl();

    QWidget *centralWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *periodLayout = new QHBoxLayout;
    QVBoxLayout *amplitudeLayout = new QVBoxLayout;

    periodLayout->addWidget(m_perSpinBox);
    periodLayout->addWidget(m_perSlider);

    QDockWidget *periodDock = new QDockWidget("Period controls");
    periodDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    periodDock->setLayout(periodLayout);

    amplitudeLayout->addWidget(m_ampSlider);
    amplitudeLayout->addWidget(m_ampSpinBox);

    QDockWidget *amplitudeDock = new QDockWidget("Amplitude controls");
    amplitudeDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    amplitudeDock->setLayout(amplitudeLayout);

    mainLayout->addWidget(m_graphView);

    mainLayout->addLayout(amplitudeLayout);
    mainLayout->addLayout(periodLayout);
    centralWidget->setLayout(mainLayout);

    setCentralWidget(centralWidget);
    resize(1280, 720);

    // Create layout for controls
    //QVBoxLayout *dockLayout = new QVBoxLayout;
    //dockLayout->addWidget(m_graphView);

    //m_slider->setMaximumWidth(width()*0.5);
    //dockLayout->addWidget(m_slider);
    //dockLayout->addWidget(m_spinBox);

    // Creates a dock
    //QDockWidget *dockWidget = new QDockWidget("Oscilloscope controls");
    //dockWidget->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    //dockWidget->setLayout(dockLayout);
    //addDockWidget(Qt::BottomDockWidgetArea, dockWidget);
    //dockWidget->setWidget(spinBox);
    //dockWidget->setWidget(slider);
    //QWidget *CentralWidget = new QWidget;
    //CentralWidget->setLayout(dockLayout);

    // Sets the graph as the applications central widget
    //setLayout(dockLayout);
    //setCentralWidget(CentralWidget);
    //resize(1280, 720);

    // Sets a timer to update the graphs contents in milliseconds
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
    timer->start(1000 * m_deltaTime);
}

MainWindow::~MainWindow() {
    // Destroys the main window
}

void MainWindow::update() {
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

    // Switch from using period/amplitude to voltage/time
    m_xAxis->setRange(0, m_period);
    m_yAxis->setRange(-m_amplitude, m_amplitude);
}

void MainWindow::setPeriod(int p) {
    m_period = p;
}

void MainWindow::setAmplitude(int a) {
    m_amplitude = a;
}

// Creates widgets for controlling the period of the (co)sine function
void MainWindow::periodControl() {
    m_perSpinBox = new QSpinBox;
    m_perSlider = new QSlider(Qt::Horizontal);

    m_perSlider->setRange(1, 60);
    m_perSpinBox->setRange(1, 60);

    connect(m_perSpinBox, SIGNAL(valueChanged(int)), m_perSlider, SLOT(setValue(int)));
    connect(m_perSlider, SIGNAL(valueChanged(int)), m_perSpinBox, SLOT(setValue(int)));
    connect(m_perSlider, SIGNAL(valueChanged(int)), this, SLOT(setPeriod(int)));

    m_perSpinBox->setValue(5);
}

// Creates widgets for controlling the amplitude of the (co)sine funtion
void MainWindow::amplitudeControl() {
    m_ampSpinBox = new QSpinBox;
    m_ampSlider = new QSlider(Qt::Vertical);

    m_ampSlider->setRange(1, 10);
    m_ampSpinBox->setRange(1, 10);

    connect(m_ampSpinBox, SIGNAL(valueChanged(int)), m_ampSlider, SLOT(setValue(int)));
    connect(m_ampSlider, SIGNAL(valueChanged(int)), m_ampSpinBox, SLOT(setValue(int)));
    connect(m_ampSlider, SIGNAL(valueChanged(int)), this, SLOT(setAmplitude(int)));

    m_ampSpinBox->setValue(2);
}
