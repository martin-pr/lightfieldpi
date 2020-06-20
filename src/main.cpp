#include <iostream>

#include <QApplication>
#include <QLabel>

#include <wiringPi.h>

#include "main_window.h"

int main(int argc, char *argv[]) {
    wiringPiSetupPhys();

    QApplication app(argc, argv);

    MainWindow win;
    //win.showFullScreen();
    win.showMaximized();

    app.exec();

    return 0;
}
