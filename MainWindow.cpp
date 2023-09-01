#include "MainWindow.h"

#include <QtMath>
#include <QTimer>
#include <QDebug>

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
    m_yAxis->setRange(-m_amplitude, m_amplitude);
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

    // Creates the voltage slider and spin box
    QSpinBox *m_spinBox = new QSpinBox;
    QSlider *m_slider = new QSlider(Qt::Horizontal);

    m_slider->setRange(1, 60);
    m_spinBox->setRange(1, 60);

    connect(m_spinBox, SIGNAL(valueChanged(int)), m_slider, SLOT(setValue(int)));
    connect(m_slider, SIGNAL(valueChanged(int)), m_spinBox, SLOT(setValue(int)));
    connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(setPeriod(int)));

    m_spinBox->setValue(5);

    QWidget *centralWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *sliderLayout = new QHBoxLayout;

    mainLayout->addWidget(m_graphView);

    sliderLayout->addWidget(m_spinBox);
    sliderLayout->addWidget(m_slider);

    mainLayout->addLayout(sliderLayout);
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
    m_xAxis->setRange(0, m_period);
}

void MainWindow::setPeriod(int p) {
    m_period = p;
}

void MainWindow::layout() {

}
