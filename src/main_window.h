#pragma once

#include <QMainWindow>

#include "camera_view.h"

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        CameraView* m_cameraView;
};
