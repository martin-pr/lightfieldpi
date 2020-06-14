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
    m_camera.setWidth(WIDTH);
    m_camera.setHeight(HEIGHT);
    m_camera.setFormat(raspicam::RASPICAM_FORMAT_BGR);

    m_camera.setUserCallback(updateCallback, this);

    if(!m_camera.open())
        std::cerr << "Error opening camera" << std::endl;

    // initialise the image for view data update
    m_data.resize(WIDTH*HEIGHT*3);
    m_image = QImage(m_data.data(), WIDTH, HEIGHT, QImage::Format_RGB888);
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
