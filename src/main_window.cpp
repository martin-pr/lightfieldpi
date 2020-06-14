#include "main_window.h"

#include <sstream>
#include <iostream>

#include <QHBoxLayout>
#include <QFormLayout>
#include <QComboBox>

namespace {
    static const std::vector<int> s_multipliers({{
        1, 2, 4, 8
    }});
}

MainWindow::MainWindow() : QMainWindow() {
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
    QFormLayout* form = new QFormLayout(controls);

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

    form->addRow("Resolution:", resolution);

    layout->addWidget(controls, 1);

    setCentralWidget(central);
}

