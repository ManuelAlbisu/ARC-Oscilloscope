#include "MainWindow.h"

#include <QTimer>
#include <QChartView>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    resize(1280, 720);

    // Populate window
    createActions();
    createToolBar();
    createMenuBar();
    createContextMenu();

    createChartView();
    createControlsDock();
    createConsoleDock();

    // Sets a timer to update the graph in milliseconds
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
    timer->start(1000 * m_deltaTime);
}

MainWindow::~MainWindow() {
    // MainWindow destructor
}

void MainWindow::createActions() {
    // Clear console action
    //m_clearConsoleAction = new QAction("&Clear");
    //m_clearConsoleAction->setShortcut("CTRL+C");
    //m_clearConsoleAction->setStatusTip("Clear previous commands from console.");
}

void MainWindow::createToolBar() {

}

void MainWindow::createMenuBar() {
    // View menu
    m_viewMenu = menuBar()->addMenu("&View");
}

void MainWindow::createContextMenu() {
    // Console context menu
    //m_console->addAction(m_clearConsoleAction);
    //m_console->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createChartView() {
    m_sine = new QLineSeries();
    m_cosine = new QLineSeries();

    // Creates the graph
    auto graphView = new QChartView();
    graphView->chart()->legend()->hide();
    graphView->chart()->setTitle("ARC Oscilloscope");

    m_xAxis = new QValueAxis();
    m_xAxis->setTitleText("Time (s");
    graphView->chart()->addAxis(m_xAxis, Qt::AlignBottom);

    m_yAxis = new QValueAxis();
    m_yAxis->setTitleText("Voltage (V");
    graphView->chart()->addAxis(m_yAxis, Qt::AlignLeft);

    // Adds (co)sine wave to the graph
    graphView->chart()->addSeries(m_sine);
    m_sine->attachAxis(m_xAxis);
    m_sine->attachAxis(m_yAxis);

    graphView->chart()->addSeries(m_cosine);
    m_cosine->attachAxis(m_xAxis);
    m_cosine->attachAxis(m_yAxis);

    // Sets bounds for wave function
    m_xAxis->setRange(0, 5);
    m_yAxis->setRange(-2, 2);

    setCentralWidget(graphView);
}

void MainWindow::createControlsDock() {
    // Supplies widgets
    amplitudeControl();
    periodControl();
    phaseControl();

    // Creates the dock
    QDockWidget *dock = new QDockWidget("Controls", this);
    dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea
                          | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // Creates layouts
    auto ampHLayout = new QHBoxLayout;
    ampHLayout->addWidget(m_ampLabel);
    ampHLayout->addWidget(m_ampSpinBox);
    ampHLayout->addWidget(m_ampSlider);

    auto perHLayout = new QHBoxLayout;
    perHLayout->addWidget(m_perLabel);
    perHLayout->addWidget(m_perSpinBox);
    perHLayout->addWidget(m_perSlider);

    auto phaseHLayout = new QHBoxLayout;
    phaseHLayout->addWidget(m_phaseLabel);
    phaseHLayout->addWidget(m_phaseSpinBox);
    phaseHLayout->addWidget(m_phaseDial);

    // Collects layouts into a single layout
    auto v = new QVBoxLayout;
    v->addLayout(ampHLayout);
    v->addLayout(perHLayout);
    v->addLayout(phaseHLayout);

    // Sets layout to a widget
    auto w = new QWidget;
    w->setLayout(v);

    // Sets widgets to dock
    dock->setWidget(w);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
    m_viewMenu->addAction(dock->toggleViewAction());
}

void MainWindow::createConsoleDock() {
    // Creates the dock
    QDockWidget *dock = new QDockWidget("Console", this);
    dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea
                          | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // Creates console
    m_console = new QListWidget;
    m_input = new QLineEdit;

    // Adds console to vertical layout
    auto v = new QVBoxLayout;
    v->addWidget(m_console);
    v->addWidget(m_input);

    // Sets layout to a widget
    auto w = new QWidget;
    w->setLayout(v);

    // Adds console widget to dock
    dock->setWidget(w);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    m_viewMenu->addAction(dock->toggleViewAction());

    // Grabs input from input box and sends it to console
    connect(m_input, SIGNAL(returnPressed()), this, SLOT(consoleCommandInput()));
}

void MainWindow::consoleCommandInput() {
    // Sends input to console box then clears input box
    auto command = m_input->text();
    m_console->addItem(command);
    execute(command);
    m_input->clear();
}

void MainWindow::execute(const QString &command) {
    if (command.toLower() == "clear")
        m_console->clear();
    else if (command.toLower() == "info") {
        m_console->addItem("----------------");
        m_console->addItem("Period: ");
        m_console->addItem("Amplitude: ");
        m_console->addItem("Phase: ");
        m_console->addItem("----------------");
    }
}

void MainWindow::update() {
    m_time = 0.0;
    m_deltaTime = 0.001;

    m_time += m_deltaTime;

    // Restarts plotting if wave reaches end of range
    if (m_time > m_period) {
        m_time = 0.0;
        m_sine->clear();
        m_cosine->clear();
    }

    // Gets y-coordinate of the (co)sine function
    qreal ySin = m_amplitude * sin((2 * M_PI / m_period) * m_time + m_phase);
    qreal yCos = m_amplitude * cos((2 * M_PI / m_period) * m_time + m_phase);

    // Plots x & y points to the graph
    m_sine->append(m_time, ySin);
    m_cosine->append(m_time, yCos);
}

void MainWindow::amplitudeControl() {
    const int initVal = 2; // Initial value
    const int upper = 10;  // Upper limit
    const int lower = 0;   // Lower limit

    // Creates widgets
    m_ampSpinBox = new QSpinBox();
    m_ampSlider = new QSlider(Qt::Horizontal);
    m_ampLabel = new QLabel("Amplitude");

    m_ampLabel->setBuddy(m_ampSpinBox);

    // Sets range of widgets
    m_ampSlider->setRange(lower, upper);
    m_ampSpinBox->setRange(lower, upper);

    // Connects widgets to each other and the amplitude
    connect(m_ampSpinBox, SIGNAL(valueChanged(int)), m_ampSlider, SLOT(setValue(int)));
    connect(m_ampSlider, SIGNAL(valueChanged(int)), m_ampSpinBox, SLOT(setValue(int)));
    connect(m_ampSlider, SIGNAL(valueChanged(int)), this, SLOT(setAmplitude(int)));

    // Sets starting value
    m_ampSpinBox->setValue(initVal);
}

void MainWindow::periodControl() {
    const int initVal = 5; // Initial value
    const int upper = 60;  // Upper limit
    const int lower = 1;   // Lower limit

    // Creates widgets
    m_perSpinBox = new QSpinBox;
    m_perSlider = new QSlider(Qt::Horizontal);
    m_perLabel = new QLabel("Period");

    m_perLabel->setBuddy(m_perSpinBox);

    // Sets range of widgets
    m_perSlider->setRange(lower, upper);
    m_perSpinBox->setRange(lower, upper);

    // Connects widgets to each other and the period
    connect(m_perSpinBox, SIGNAL(valueChanged(int)), m_perSlider, SLOT(setValue(int)));
    connect(m_perSlider, SIGNAL(valueChanged(int)), m_perSpinBox, SLOT(setValue(int)));
    connect(m_perSlider, SIGNAL(valueChanged(int)), this, SLOT(setPeriod(int)));

    // Sets starting value
    m_perSpinBox->setValue(initVal);
}

void MainWindow::phaseControl() {
    const int initVal = 0; // Initial value
    const int upper = 10;  // Upper limit
    const int lower = 0;   // Lower limit

    // Creates widgets
    m_phaseDial = new QDial();
    m_phaseSpinBox = new QSpinBox();
    m_phaseLabel = new QLabel("Phase");

    m_phaseLabel->setBuddy(m_phaseSpinBox);

    // Sets range of widgets
    m_phaseDial->setRange(lower, upper);

    // Connects widgets to each other and the phase
    connect(m_phaseSpinBox, SIGNAL(valueChanged(int)), m_phaseDial, SLOT(setValue(int)));
    connect(m_phaseDial, SIGNAL(valueChanged(int)), m_phaseSpinBox, SLOT(setValue(int)));
    connect(m_phaseDial, SIGNAL(valueChanged(int)), this, SLOT(setPhase(int)));

    // Sets starting value
    m_phaseSpinBox->setValue(initVal);
}

void MainWindow::setAmplitude(int amplitude) {
    m_amplitude = amplitude;
}

void MainWindow::setPeriod(int period) {
    m_period = period;
}

void MainWindow::setPhase(int phase) {
    m_phase = phase;
}
