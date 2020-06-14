#include <iostream>

#include <QApplication>
#include <QLabel>

#include "main_window.h"
#include "camera_view.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow win;
    //win.showFullScreen();
    win.showMaximized();

    CameraView* view = new CameraView();
    win.setCentralWidget(view);

    app.exec();

    return 0;
}
