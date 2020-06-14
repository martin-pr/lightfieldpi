#include "main_window.h"

#include <sstream>

#include <QHBoxLayout>
#include <QFormLayout>
#include <QComboBox>

namespace {
    static const std::vector<int> s_multipliers({{
        1, 2, 4, 8
    }});
}

#define MAX_MULTIPLIER 8

MainWindow::MainWindow() : QMainWindow() {
    QWidget* central = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(central);
    layout->setContentsMargins(0,0,0,0);

    m_cameraView = new CameraView();
    layout->addWidget(m_cameraView, 4);

    QWidget* controls = new QWidget();
    QFormLayout* form = new QFormLayout(controls);

    auto baseResolution = m_cameraView->baseResolution();

    QComboBox* resolution = new QComboBox();
    for(auto mult: s_multipliers) {
        std::stringstream ss;
        ss << (baseResolution.first * (mult)) << " x " << (baseResolution.second * (mult));

        resolution->addItem(ss.str().c_str());
    }

    connect(resolution, (void (QComboBox::*)(int))(&QComboBox::currentIndexChanged), [this](int val) {
        m_cameraView->setResolutionMultiplier(s_multipliers[val]);
    });

    form->addRow("Resolution:", resolution);

    layout->addWidget(controls, 1);

    setCentralWidget(central);
}
