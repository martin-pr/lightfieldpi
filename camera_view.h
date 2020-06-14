#pragma once

#include <QWidget>

#include <raspicam/raspicam.h>

class CameraView : public QWidget {
    public:
        CameraView(QWidget* parent = nullptr);

    protected:
        virtual void paintEvent(QPaintEvent* e) override;

    private:
        raspicam::RaspiCam m_camera;

        QImage m_image;
        std::vector<unsigned char> m_data;
};
