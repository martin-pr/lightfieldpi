// #pragma once

#include <memory>
#include <utility>

#include <QObject>
#include <QImage>

#include <raspicam/raspicam.h>

// #include <functional>

// derived from a QObject - Qt-specific implementation
// noncopyable + assert one instance
// returns a QImage
// fires a signal for refresh - caught by MainWindow and redirected to CameraView for refresh (if continuous)
// allows to capture a static image
// two constructors - static exposure, dynamic exposure

// QStackedWidget for 2 modes - capture and setup
//   - setup - set resolution and capture mode
//   - capture - remember exposure, start the movement, show progress

// shutter speed of 0 = automatic adjustment

// NAKE SHUTTER SPEED UI!

class Camera : public QObject {
    Q_OBJECT

    public:
        Camera();

        ~Camera();

        QImage capture();

        /// returns the current exact exposure time in microseconds
        unsigned exposure() const;
        /// sets the exposure time (in microseconds); 0 = automatic exposure
        void setExposure(unsigned exp);

        static std::pair<int, int> baseResolution();

        void setResolutionMultiplier(int mult);

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
