#include <iostream>

#include <QApplication>
#include <QLabel>

#include "main_window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow win;
    //win.showFullScreen();
    win.showMaximized();

    QLabel* label = new QLabel();
    label->setText("Hello world!");
    label->setAlignment(Qt::AlignCenter);
    win.setCentralWidget(label);

    app.exec();

    std::cout << "Hello world!" << std::endl;
    return 0;
}