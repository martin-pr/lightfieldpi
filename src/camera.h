#pragma once

#include <memory>
#include <utility>

#include <QObject>
#include <QImage>

#include <raspicam/raspicam.h>

class Camera : public QObject {
    Q_OBJECT

    public:
        // a representation of the camera state - white balance, exposure + other values
        class State {
            private:
                State();

                int m_brightness, m_iso, m_sharpness, m_contrast, m_saturation, m_shutterSpeed;
                raspicam::RASPICAM_EXPOSURE m_exposure;
                raspicam::RASPICAM_AWB m_awb;
                float m_awbRed, m_awbBlue;
                raspicam::RASPICAM_METERING m_metering;

            friend class Camera;
        };

        Camera();

        ~Camera();

        QImage capture();

        static std::pair<int, int> baseResolution();
        void setResolutionMultiplier(int mult);

        /// sets the camera state
        void setState(const State& state);
        /// gets the current camera state (all settings and modes, including automatic)
        State state() const;
        /// gets the current state values, reproducing manual setup (using this state will disable automatic exposure, white balance and others!)
        State manualState() const;

    signals:
        void imageReady();

    private:
        // noncopyable
        Camera& operator = (const Camera&) = delete;
        Camera(const Camera&) = delete;

        static void updateCallback(void* data);

        raspicam::RaspiCam m_camera;

        QImage m_image;
        std::vector<unsigned char> m_data;
};
