#pragma once

#include <QMainWindow>

#include "camera_view.h"
#include "camera.h"

#include "motor.h"

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        CameraView* m_cameraView;

        Camera m_camera;

        Motor m_motor1, m_motor2;

        bool m_continuous;
        int m_delay;
};
