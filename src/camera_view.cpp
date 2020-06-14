#include "camera_view.h"

#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>

#include <QPainter>

// /usr/local/include/raspicam/raspicamtypes.h
// /usr/local/include/raspicam/raspicam.h
// /usr/local/include/raspicam/raspicam_cv.h
// /usr/local/include/raspicam/raspicam_still_cv.h

#define WIDTH 320
#define HEIGHT 240

namespace {
    void updateCallback(void* data) {
        CameraView* view = (CameraView*)data;
        view->update();
    }
}

CameraView::CameraView(QWidget* parent) : QWidget(parent) {
    m_camera.setFormat(raspicam::RASPICAM_FORMAT_BGR);

    m_camera.setUserCallback(updateCallback, this);

    m_camera.setWidth(WIDTH);
    m_camera.setHeight(HEIGHT);

    m_data.resize(WIDTH*HEIGHT*3);
    m_image = QImage(m_data.data(), WIDTH, HEIGHT, QImage::Format_RGB888);

    if(!m_camera.open())
        std::cerr << "Error opening camera" << std::endl;
}

void CameraView::paintEvent(QPaintEvent* e) {
    // update the camera data
    // TODO: different thread?
    m_camera.grab();
    m_camera.retrieve(m_data.data());

    // figure out the size
    const float aspect = (float)width() / (float)height();
    const float imgAspect = (float)WIDTH / (float)HEIGHT;

    QPoint origin;
    QSize size;
    if(aspect > imgAspect) {
        origin = QPoint((width() - width()/aspect*imgAspect) / 2, 0);
        size = QSize(width() / aspect * imgAspect, height());
    }
    else {
        origin = QPoint(0, (height() - height()*aspect/imgAspect) / 2);
        size = QSize(width(), height() * aspect / imgAspect);
    }

    // paint the acquired image
    QPainter painter(this);
    painter.drawImage(QRect(origin, size), m_image);
}

void CameraView::setResolutionMultiplier(int multiplier) {
    m_camera.release();

    m_camera.setWidth(WIDTH*multiplier);
    m_camera.setHeight(HEIGHT*multiplier);

    m_data.resize(WIDTH*HEIGHT*3*multiplier*multiplier);

    if(!m_camera.open())
        std::cerr << "Error opening camera" << std::endl;

    m_image = QImage(m_data.data(), WIDTH*multiplier, HEIGHT*multiplier, QImage::Format_RGB888);
}

std::pair<int, int> CameraView::baseResolution() {
    return std::make_pair(WIDTH, HEIGHT);
}
