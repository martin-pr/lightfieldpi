#include "camera.h"

#include <iostream>

#define WIDTH 320
#define HEIGHT 240

namespace {
    static int s_instanceCounter = 0;
}

Camera::Camera() {
    m_camera.setFormat(raspicam::RASPICAM_FORMAT_BGR);

    m_camera.setWidth(WIDTH);
    m_camera.setHeight(HEIGHT);

    m_data.resize(WIDTH*HEIGHT*3);
    m_image = QImage(m_data.data(), WIDTH, HEIGHT, QImage::Format_RGB888);

    m_camera.setUserCallback(updateCallback, this);

    if(!m_camera.open())
        std::cerr << "Error opening camera" << std::endl;
    else {
        assert(s_instanceCounter == 0);
        ++s_instanceCounter;
    }

    // m_camera.grab();
}

Camera::~Camera() {
    assert(s_instanceCounter == 1);
    --s_instanceCounter;

    m_camera.release();
}

QImage Camera::capture() {
    // update the camera data
    // TODO: different thread?
    // m_camera.grab();
    m_camera.retrieve(m_data.data());

    // just return the image already bound to this data
    return m_image;
}

unsigned Camera::exposure() const {
    return m_camera.getShutterSpeed();
}

void Camera::setExposure(unsigned exp) {
    m_camera.setShutterSpeed(exp);
}

void Camera::updateCallback(void* data) {
    Camera* cam = (Camera*)data;

    emit cam->imageReady();
}

std::pair<int, int> Camera::baseResolution() {
    return std::make_pair(WIDTH, HEIGHT);
}

void Camera::setResolutionMultiplier(int multiplier) {
    m_camera.release();

    m_camera.setWidth(WIDTH*multiplier);
    m_camera.setHeight(HEIGHT*multiplier);

    m_data.resize(WIDTH*HEIGHT*3*multiplier*multiplier);
    m_image = QImage(m_data.data(), WIDTH*multiplier, HEIGHT*multiplier, QImage::Format_RGB888);

    if(!m_camera.open())
        std::cerr << "Error opening camera" << std::endl;
}
