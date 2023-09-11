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

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    resize(1280, 720);
    // Populate window
    createChartView();
    createConsoleDock();
    createControlDock();
    // Sets a timer to update the graphs contents in milliseconds
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
    timer->start(1000 * m_deltaTime);
}

MainWindow::~MainWindow() {
    // Destroys the main window
}

void MainWindow::createChartView() {
    // Set variables
    //m_amplitude = 2.0;
    //m_period = 5.0;
    //m_phase = 0.0;

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

    setCentralWidget(m_graphView);
}

void MainWindow::createConsoleDock() {
    QDockWidget *dock = new QDockWidget(tr("Controls"), this);
    dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_console = new QListWidget;
    m_input = new QLineEdit;

    auto v = new QVBoxLayout;
    v->addWidget(m_console);
    v->addWidget(m_input);

    auto w = new QWidget;
    w->setLayout(v);

    dock->setWidget(w);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    //m_viewMenu->addAction(dock->toggleViewAction());

    connect(m_input, SIGNAL(returnPressed()), this, SLOT(consoleCommandInput()));
}

void MainWindow::createControlDock() {
    QDockWidget *dock = new QDockWidget(tr("Controls"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    m_perSpinBox = new QSpinBox;
    m_perSlider = new QSlider(Qt::Horizontal);
    m_perLabel = new QLabel("Period");

    m_perLabel->setBuddy(m_perSpinBox);

    m_perSlider->setRange(1, 60);
    m_perSpinBox->setRange(1, 60);

    connect(m_perSpinBox, SIGNAL(valueChanged(int)), m_perSlider, SLOT(setValue(int)));
    connect(m_perSlider, SIGNAL(valueChanged(int)), m_perSpinBox, SLOT(setValue(int)));
    connect(m_perSlider, SIGNAL(valueChanged(int)), this, SLOT(setPeriod(int)));

    m_perSpinBox->setValue(5);

    auto v = new QVBoxLayout;
    v->addWidget(m_perSpinBox);
    v->addWidget(m_perSlider);
    v->addWidget(m_perLabel);

    auto w = new QWidget;
    w->setLayout(v);

    dock->setWidget(w);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    //m_viewMenu->addAction(dock->toggleViewAction());
}

void MainWindow::consoleCommandInput() {
    auto command = m_input->text();
    m_console->addItem(command);
    execute(command);
    m_input->clear();
}

void MainWindow::execute(const QString &command) {
    if (command == "clear")
        m_console->clear();
    else if (command == "info") {
        m_console->addItem("Many is awesome!");
        m_console->addItem("Luiz is great!");
    }
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

void MainWindow::setPeriod(int period) {
    m_period = period;
}

void MainWindow::setAmplitude(int amplitude) {
    m_amplitude = amplitude;
}

void MainWindow::setPhase(int phase) {
    m_phase = phase;
}

// Creates widgets for controlling the period of the (co)sine function
void MainWindow::periodControl() {

}

// Creates widgets for controlling the amplitude of the (co)sine funtion
void MainWindow::amplitudeControl() {
    m_ampSpinBox = new QSpinBox;
    m_ampSlider = new QSlider(Qt::Horizontal);
    m_ampLabel = new QLabel("Amplitude");

    m_ampLabel->setBuddy(m_ampSpinBox);

    m_ampSlider->setRange(1, 10);
    m_ampSpinBox->setRange(1, 10);

    connect(m_ampSpinBox, SIGNAL(valueChanged(int)), m_ampSlider, SLOT(setValue(int)));
    connect(m_ampSlider, SIGNAL(valueChanged(int)), m_ampSpinBox, SLOT(setValue(int)));
    connect(m_ampSlider, SIGNAL(valueChanged(int)), this, SLOT(setAmplitude(int)));

    m_ampSpinBox->setValue(2);
}

void MainWindow::phaseControl() {
    m_phaseDial = new QDial;
    m_phaseSpinBox = new QSpinBox;
    m_phaseLabel = new QLabel("Phase");

    m_phaseLabel->setBuddy(m_phaseSpinBox);

    m_phaseDial->setRange(0, 10);

    connect(m_phaseSpinBox, SIGNAL(valueChanged(int)), m_phaseDial, SLOT(setValue(int)));
    connect(m_phaseDial, SIGNAL(valueChanged(int)), m_phaseSpinBox, SLOT(setValue(int)));
    connect(m_phaseDial, SIGNAL(valueChanged(int)), this, SLOT(setPhase(int)));

    m_phaseSpinBox->setValue(0);
}
