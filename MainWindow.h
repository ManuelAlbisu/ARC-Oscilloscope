#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLineSeries>
#include <QValueAxis>
#include <QListWidget>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QDial>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void update();
    void periodControl();
    void amplitudeControl();
    void phaseControl();
    void consoleCommandInput();

    // Accessors
    void setAmplitude(int amplitude);
    void setPeriod(int period);
    void setPhase(int phase);

private:
    void createActions();
    void createToolBar();
    void createMenuBar();
    void createContextMenu();
    void createChartView();
    void createControlsDock();
    void createConsoleDock();
    void execute(const QString &command);

    // Actions
    QAction *m_clearConsoleAction;

    // Menus
    QMenu *m_viewMenu;

    // Wave function
    QLineSeries *m_sine;
    QLineSeries *m_cosine;

    qreal m_time;
    qreal m_deltaTime;
    qreal m_amplitude;
    qreal m_period;
    qreal m_phase;

    // Graph
    QValueAxis *m_xAxis;
    QValueAxis *m_yAxis;

    // Controls
    QSpinBox *m_perSpinBox;
    QSlider *m_perSlider;
    QLabel *m_perLabel;

    QSpinBox *m_ampSpinBox;
    QSlider *m_ampSlider;
    QLabel *m_ampLabel;

    QSpinBox *m_phaseSpinBox;
    QDial *m_phaseDial;
    QLabel *m_phaseLabel;

    // Console
    QListWidget *m_console;
    QLineEdit *m_input;
};

#endif // MAINWINDOW_H
