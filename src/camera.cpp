#include "camera.h"

#include <iostream>

#define WIDTH 320
#define HEIGHT 240

namespace {
    static int s_instanceCounter = 0;
}

Camera::Camera() {
    m_camera.setFormat(raspicam::RASPICAM_FORMAT_BGR);

    // leave the state on default settings - everything automatic

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

Camera::State::State() {
}

Camera::State Camera::state() const {
    State s;

    s.m_brightness = m_camera.getBrightness();
    s.m_iso = m_camera.getISO();
    s.m_sharpness = m_camera.getSharpness();
    s.m_contrast = m_camera.getContrast();
    s.m_saturation = m_camera.getSaturation();
    s.m_shutterSpeed = m_camera.getShutterSpeed();

    s.m_exposure = m_camera.getExposure();
    s.m_awb = m_camera.getAWB();
    s.m_awbRed = m_camera.getAWBG_red();
    s.m_awbBlue = m_camera.getAWBG_blue();
    s.m_metering = m_camera.getMetering();

    return s;
}

Camera::State Camera::manualState() const {
    State s;

    s.m_brightness = m_camera.getBrightness();
    s.m_iso = m_camera.getISO();
    s.m_sharpness = m_camera.getSharpness();
    s.m_contrast = m_camera.getContrast();
    s.m_saturation = m_camera.getSaturation();
    s.m_shutterSpeed = m_camera.getShutterSpeed();

    s.m_exposure = raspicam::RASPICAM_EXPOSURE_OFF;
    s.m_awb = raspicam::RASPICAM_AWB_OFF;
    s.m_awbRed = m_camera.getAWBG_red();
    s.m_awbBlue = m_camera.getAWBG_blue();
    s.m_metering = m_camera.getMetering();

    return s;
}

void Camera::setState(const State& s) {
    m_camera.setExposure(s.m_exposure);
    m_camera.setShutterSpeed(s.m_exposure == raspicam::RASPICAM_EXPOSURE_OFF ? s.m_shutterSpeed : 0); // 0 = automatic

    m_camera.setBrightness(s.m_brightness);
    m_camera.setISO(s.m_iso);
    m_camera.setSharpness(s.m_sharpness);
    m_camera.setContrast(s.m_contrast);
    m_camera.setSaturation(s.m_saturation);

    m_camera.setAWB(s.m_awb);
    if(s.m_awb == raspicam::RASPICAM_AWB_OFF)
        m_camera.setAWB_RB(s.m_awbRed, s.m_awbBlue);
    m_camera.setMetering(s.m_metering);
}
