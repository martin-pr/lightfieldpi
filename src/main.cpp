#include <iostream>

#include <QApplication>
#include <QLabel>

#include "main_window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow win;
    //win.showFullScreen();
    win.showMaximized();

    app.exec();

    return 0;
}
