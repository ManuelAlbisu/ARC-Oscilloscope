#include "MainWindow.h"

#include <QApplication>
#include <QSlider>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;

    // Creates a slider
    QSlider *xSlider = new QSlider(Qt::Horizontal);
    QSlider *ySlider = new QSlider(Qt::Horizontal);

    xSlider->setRange(0, 100);
    QObject::connect(xSlider, SIGNAL(valueChanged(int)), ySlider, SLOT(setValue(int)));
    xSlider->setValue(5);

    ySlider->setRange(-10, 10);
    QObject::connect(ySlider, SIGNAL(valueChanged(int)), xSlider, SLOT(setValue(int)));
    ySlider->setValue(2);

    // Creates horizontal box layout
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(xSlider);
    layout->addWidget(ySlider);
    window.setLayout(layout);

    window.show();
    return app.exec();
}
