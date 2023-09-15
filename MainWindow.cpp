#include "MainWindow.h"

#include <QTimer>
#include <QChartView>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

// Testing
#include <QStatusBar>
#include <QRandomGenerator>
#include <QEasingCurve>
#include <QDebug>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    resize(1920, 1080);

    // Populate window
    createActions();
    createToolBar();
    createMenuBar();
    createContextMenu();

    createChartView();
    createControlsDock();
    createConsoleDock();
}

MainWindow::~MainWindow() {

}

void MainWindow::createActions() {
    // Save graph action
    m_saveGraphAction = new QAction("Save Graph...");
    m_saveGraphAction->setShortcut(QKeySequence::Save);
    m_saveGraphAction->setStatusTip("Save graph's settings.");
    connect(m_saveGraphAction, SIGNAL(triggered(bool)), this, SLOT(saveGraph()));

    // Open graph action
    m_openGraphAction = new QAction("Open Graph...");
    m_openGraphAction->setShortcut(QKeySequence::Open);
    m_openGraphAction->setStatusTip("Open a saved graph.");
    connect(m_openGraphAction, SIGNAL(triggered(bool)), this, SLOT(openGraph()));

    // Reset graph action
    m_resetGraphAction = new QAction("Reset Graph...");
    m_resetGraphAction->setShortcut(QKeySequence::New);
    m_resetGraphAction->setStatusTip("Reset the graph to it's default values.");
    connect(m_resetGraphAction, SIGNAL(triggered(bool)), this, SLOT(resetGraph()));

    // Exit application action
    m_exitAction = new QAction("Exit");
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip("Exit application.");
    connect(m_exitAction, SIGNAL(triggered(bool)), this, SLOT(exit()));

    // Toggle sine wave action
    m_sinToggleAction = new QAction("Sine");
    m_sinToggleAction->setStatusTip("Toggle visibility of sine wave.");
    connect(m_sinToggleAction, SIGNAL(triggered(bool)), this, SLOT(sineToggle()));

    // Toggle cosine wave action
    m_cosToggleAction = new QAction("Cosine");
    m_cosToggleAction->setStatusTip("Toggle visibility of cosine wave.");
    connect(m_cosToggleAction, SIGNAL(triggered(bool)), this, SLOT(cosineToggle()));
}

void MainWindow::createMenuBar() {
    // File menu
    m_fileMenu = menuBar()->addMenu("File");
    m_fileMenu->addAction(m_saveGraphAction);
    m_fileMenu->addAction(m_openGraphAction);
    m_fileMenu->addAction(m_resetGraphAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    // View menu
    m_viewMenu = menuBar()->addMenu("View");
    m_viewMenu->addAction(m_fileToolBar->toggleViewAction());
    m_viewMenu->addAction(m_waveToolBar->toggleViewAction());
}

void MainWindow::createToolBar() {
    // File toolbar
    m_fileToolBar = addToolBar("Files Bar");
    m_fileToolBar->addAction(m_saveGraphAction);
    m_fileToolBar->addAction(m_openGraphAction);
    m_fileToolBar->addAction(m_resetGraphAction);

    // Toggle toolbar
    m_waveToolBar = addToolBar("Wave Bar");
    m_waveToolBar->addAction(m_sinToggleAction);
    m_waveToolBar->addAction(m_cosToggleAction);
}

void MainWindow::createContextMenu() {

}

void MainWindow::createChartView() {
    // Supplies wave functions
    waveFunctions();

    // Creates the graph and graph view
    auto chart = new QChart();
    chart->legend()->hide();

    auto chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Graph decoration
    chart->setTitle("ARC Oscilloscope");
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setAnimationDuration(20000);
    chart->setAnimationEasingCurve(QEasingCurve::Linear);

    // Creates the axes
    m_xAxis = new QValueAxis();
    m_xAxis->setTitleText("Time (s)");
    chartView->chart()->addAxis(m_xAxis, Qt::AlignBottom);

    m_yAxis = new QValueAxis();
    m_yAxis->setTitleText("Voltage (V)");
    chartView->chart()->addAxis(m_yAxis, Qt::AlignLeft);

    // Adds wave functions to the graph
    chartView->chart()->addSeries(m_sine);
    m_sine->attachAxis(m_xAxis);
    m_sine->attachAxis(m_yAxis);

    chartView->chart()->addSeries(m_cosine);
    m_cosine->attachAxis(m_xAxis);
    m_cosine->attachAxis(m_yAxis);

    setCentralWidget(chartView);
}

void MainWindow::createControlsDock() {
    // Supplies widgets
    waveControl();
    amplitudeControl();
    periodControl();
    phaseControl();
    zoomControl();

    // Creates the dock
    QDockWidget *dock = new QDockWidget("Controls", this);
    dock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea
                          | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // Creates layouts
    auto ampHLayout = new QHBoxLayout();
    ampHLayout->addWidget(m_ampLabel);
    ampHLayout->addWidget(m_ampSpinBox);
    ampHLayout->addWidget(m_ampSlider);

    auto perHLayout = new QHBoxLayout();
    perHLayout->addWidget(m_perLabel);
    perHLayout->addWidget(m_perSpinBox);
    perHLayout->addWidget(m_perSlider);

    auto sliderVLayout = new QVBoxLayout();
    sliderVLayout->addLayout(ampHLayout);
    sliderVLayout->addLayout(perHLayout);

    auto sinCheckBoxHLayout = new QHBoxLayout();
    sinCheckBoxHLayout->addWidget(m_sinLabel);
    sinCheckBoxHLayout->addWidget(m_sinCheckBox);

    auto cosCheckBoxHLayout = new QHBoxLayout();
    cosCheckBoxHLayout->addWidget(m_cosLabel);
    cosCheckBoxHLayout->addWidget(m_cosCheckBox);

    auto checkBoxVLayout = new QVBoxLayout();
    checkBoxVLayout->addLayout(sinCheckBoxHLayout);
    checkBoxVLayout->addLayout(cosCheckBoxHLayout);

    auto phaseHLayout = new QHBoxLayout();
    phaseHLayout->addWidget(m_phaseLabel);
    phaseHLayout->addWidget(m_phaseSpinBox);
    phaseHLayout->addWidget(m_phaseDial);

    // Collects layouts into a single layout
    auto h = new QHBoxLayout();
    h->addLayout(checkBoxVLayout);
    h->addLayout(sliderVLayout);
    h->addLayout(phaseHLayout);

    // Sets layout to a widget
    auto w = new QWidget();
    w->setLayout(h);

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
    m_console = new QListWidget();
    m_input = new QLineEdit();

    // Adds console to vertical layout
    auto v = new QVBoxLayout();
    v->addWidget(m_console);
    v->addWidget(m_input);

    // Sets layout to a widget
    auto w = new QWidget();
    w->setLayout(v);

    // Adds console widget to dock
    dock->setWidget(w);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    m_viewMenu->addAction(dock->toggleViewAction());

    // Prompts user for help at application start
    m_console->addItem("Type 'list' for available commands.");

    // Grabs input from input box and sends it to console
    connect(m_input, SIGNAL(returnPressed()), this, SLOT(consoleCommandInput()));
}

void MainWindow::saveGraph() {

}

void MainWindow::openGraph() {

}

void MainWindow::resetGraph() {

}

void MainWindow::exit() {
    QApplication::quit();
}

void MainWindow::consoleCommandInput() {
    // Sends input to console box then clears input box
    auto command = m_input->text();
    m_console->addItem(command);
    execute(command);
    m_input->clear();
}

void MainWindow::execute(const QString &command) {
    static QRegularExpression re("\\s+");
    QStringList list = command.split(re);

    if (command.toLower() == "clear")
        m_console->clear();
    else if (command.toLower() == "save")
        saveGraph();
    else if (command.toLower() == "open")
        openGraph();
    else if (command.toLower() == "reset")
        resetGraph();
    else if (command.toLower() == "quit" | command.toLower() == "exit")
        exit();
    else if (command.toLower() == "info") {
        m_console->addItem("----------------");
        m_console->addItem("Amplitude: " + QString::number(amplitude()));
        m_console->addItem("Period: " + QString::number(period()));
        m_console->addItem("Phase: " + QString::number(phase()));
        m_console->addItem("----------------");
    } else if (command.toLower() == "list" | command.toLower() == "ls") {
        m_console->addItem("----------------");
        m_console->addItem("set amplitude <int>: sets amplitude to a specified value. ");
        m_console->addItem("set period <int>: sets period to a specified value. ");
        m_console->addItem("set phase <int>: sets phase to a specified value. ");
        m_console->addItem("save: saves current settings to file.");
        m_console->addItem("open: opens graph from file.");
        m_console->addItem("reset: resets the graph.");
        m_console->addItem("info: lists values of all variables.");
        m_console->addItem("clear: clears console.");
        m_console->addItem("quit: quits the application.");
        m_console->addItem("----------------");
    } else if (list.size() == 3 && list.at(0).toLower() == "set") {
        if (list.at(1).toLower() == "amplitude") {
            auto amplitude = list.at(2).toInt();
            setAmplitude(amplitude);
            m_ampSpinBox->setValue(amplitude);
        } else if (list.at(1).toLower() == "period") {
            auto period = list.at(2).toInt();
            setPeriod(period);
            m_perSpinBox->setValue(period);
        } else if (list.at(1).toLower() == "phase") {
            auto phase = list.at(2).toInt();
            setPhase(phase);
            m_phaseSpinBox->setValue(phase);
        } else
            statusBar()->showMessage(("ERROR: '" + list.at(1) + "' value unknown."), 5000);
    }
}

void MainWindow::waveFunctions() {
    amplitudeControl();
    periodControl();
    phaseControl();

    // Sine wave function
    m_sine = new QLineSeries();

    for (int i = 0; i < 500; ++i) {
        //QPointF sine((qreal) phase, amplitude * qSin(2 * M_PI / period + phase));
        QPointF sine((qreal) i, amplitude() * qSin(((2 * M_PI / period()) * i) + phase()));
        sine.ry() += QRandomGenerator::global()->bounded(1);
        *m_sine << sine;
    }

    // Cosine wave function
    m_cosine = new QLineSeries();
    for (int i = 0; i < 500; ++i) {
        QPointF cosine((qreal) i, amplitude() * qCos(((2 * M_PI / period()) * i) + phase()));
        cosine.ry() += QRandomGenerator::global()->bounded(1);
        *m_cosine << cosine;
    }
}

void MainWindow::waveControl() {
    // Creates check boxes for wave functions
    m_sinCheckBox = new QCheckBox();
    m_sinLabel = new QLabel("Sine:");
    m_sinCheckBox->setChecked(true);
    m_sinLabel->setBuddy(m_sinCheckBox);

    m_cosCheckBox = new QCheckBox();
    m_cosLabel = new QLabel("Cosine:");
    m_cosCheckBox->setChecked(true);
    m_cosLabel->setBuddy(m_cosCheckBox);

    connect(m_sinCheckBox, SIGNAL(stateChanged(int)), this, SLOT(sineToggle()));
    connect(m_cosCheckBox, SIGNAL(stateChanged(int)), this, SLOT(cosineToggle()));
}

void MainWindow::amplitudeControl() {
    const int initVal = 2; // Initial value
    const int upper = 10;  // Uppper limit
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
    setAmplitude(initVal);
    m_ampSpinBox->setValue(amplitude());
}

void MainWindow::periodControl() {
    const int initVal = 30; // Initial value
    const int upper = 60;   // Upper limit
    const int lower = 1;    // Lower limit

    // Creates widgets
    m_perSpinBox = new QSpinBox();
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
    setPeriod(initVal);
    m_perSpinBox->setValue(period());
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
    setPhase(initVal);
    m_phaseSpinBox->setValue(phase());
}

void MainWindow::zoomControl() {

}

void MainWindow::sineToggle() {
    if (m_sine->isVisible())
        m_sine->hide();
    else
        m_sine->show();
}

void MainWindow::cosineToggle() {
    if (m_cosine->isVisible())
        m_cosine->hide();
    else
        m_cosine->show();
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

qreal MainWindow::amplitude() {
    return m_amplitude;
}

qreal MainWindow::period() {
    return m_period;
}

qreal MainWindow::phase() {
    return m_phase;
}
