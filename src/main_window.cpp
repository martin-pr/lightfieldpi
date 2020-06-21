#include "main_window.h"

#include <sstream>
#include <iostream>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

#include "config.h"

namespace {
    static const std::vector<int> s_multipliers({{
        1, 2, 4, 8
    }});

    QPushButton* makeButton(QString label, std::function<void()> callback, int xsize = 40) {
        QPushButton* button = new QPushButton(label);
        button->setMinimumSize(QSize(0, 0));
        button->setMaximumSize(QSize(xsize, 30));
        QPushButton::connect(button, &QPushButton::pressed, callback);
        return button;
    }
}

MainWindow::MainWindow() : QMainWindow(), m_motor1(M1_ENABLE, M1_STEP, M1_DIR, M1_ENDSTOP, M1_DIR_INVERT), m_motor2(M2_ENABLE, M2_STEP, M2_DIR, M2_ENDSTOP, M2_DIR_INVERT) {
    // setup camera
    connect(&m_camera, &Camera::imageReady, [this]() {
        auto img = m_camera.capture();
        m_cameraView->showImage(img);
    });

    // setup the widgets
    QWidget* central = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(central);
    layout->setContentsMargins(0,0,0,0);

    m_cameraView = new CameraView();
    layout->addWidget(m_cameraView, 4);

    QWidget* controls = new QWidget();
    QVBoxLayout* form = new QVBoxLayout(controls);

    QComboBox* resolution = new QComboBox();
    {
        auto baseResolution = Camera::baseResolution();
        for(auto mult: s_multipliers) {
            std::stringstream ss;
            ss << (baseResolution.first * (mult)) << " x " << (baseResolution.second * (mult));

            resolution->addItem(ss.str().c_str());
        }
    }

    connect(resolution, (void (QComboBox::*)(int))(&QComboBox::currentIndexChanged), [this](int val) {
        assert(val >= 0 && val < s_multipliers.size());
        m_camera.setResolutionMultiplier(s_multipliers[val]);
    });

    form->addWidget(new QLabel("Preview:"));
    form->addWidget(resolution);

    // motor moves
    QHBoxLayout* m1_moves = new QHBoxLayout();
    m1_moves->addWidget(makeButton("Home", [this]() { m_motor1.home(); }, 60));
    m1_moves->addWidget(makeButton("-500", [this]() { m_motor1.move(-500); }));
    m1_moves->addWidget(makeButton("-100", [this]() { m_motor1.move(-100); }));
    m1_moves->addWidget(makeButton("+100", [this]() { m_motor1.move(100); }));
    m1_moves->addWidget(makeButton("+500", [this]() { m_motor1.move(500); }));

    form->addWidget(new QLabel("Motor 1 moves:"));
    form->addLayout(m1_moves);

    QHBoxLayout* m2_moves = new QHBoxLayout();
    m2_moves->addWidget(makeButton("Home", [this]() { m_motor2.home(); }, 60));
    m2_moves->addWidget(makeButton("-500", [this]() { m_motor2.move(-500); }));
    m2_moves->addWidget(makeButton("-100", [this]() { m_motor2.move(-100); }));
    m2_moves->addWidget(makeButton("+100", [this]() { m_motor2.move(100); }));
    m2_moves->addWidget(makeButton("+500", [this]() { m_motor2.move(500); }));

    form->addWidget(new QLabel("Motor 2 moves:"));
    form->addLayout(m2_moves);

    // filler
    form->addWidget(new QWidget(), 1);

    layout->addWidget(controls, 1);

    setCentralWidget(central);
}

