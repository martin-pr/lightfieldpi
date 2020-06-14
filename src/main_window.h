#pragma once

#include <QMainWindow>

#include "camera_view.h"
#include "camera.h"

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        CameraView* m_cameraView;

        Camera m_camera;
};
