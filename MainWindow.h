#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QApplication>
#include <QLineSeries>
#include <QValueAxis>
#include <QListWidget>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QDial>
#include <QCheckBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void saveGraph();
    void openGraph();
    void resetGraph();
    void exit();
    void consoleCommandInput();
    void execute(const QString &command);
    void waveControl();
    void amplitudeControl();
    void periodControl();
    void phaseControl();
    void zoomControl();
    void sineToggle();
    void cosineToggle();

    // Accessors
    void setAmplitude(int amplitude);
    void setPeriod(int period);
    void setPhase(int phase);
    qreal amplitude();
    qreal period();
    qreal phase();

private:
    void waveFunctions();
    void createActions();
    void createMenuBar();
    void createToolBar();
    void createContextMenu();
    void createChartView();
    void createControlsDock();
    void createConsoleDock();

    // Actions
    QAction *m_saveGraphAction;
    QAction *m_openGraphAction;
    QAction *m_resetGraphAction;
    QAction *m_exitAction;
    QAction *m_sinToggleAction;
    QAction *m_cosToggleAction;

    // Menus
    QMenu *m_fileMenu;
    QMenu *m_viewMenu;

    // Toolbars
    QToolBar *m_fileToolBar;
    QToolBar *m_waveToolBar;

    // Wave functions
    QLineSeries *m_sine;
    QLineSeries *m_cosine;

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

    QCheckBox *m_sinCheckBox;
    QLabel *m_sinLabel;
    QCheckBox *m_cosCheckBox;
    QLabel *m_cosLabel;

    // Console
    QListWidget *m_console;
    QLineEdit *m_input;
};

#endif // MAINWINDOW_H
