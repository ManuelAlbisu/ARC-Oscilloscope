#include "MainWindow.h"

#include <QApplication>
#include <QSlider>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
