#include "camera_view.h"

#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>

#include <QPainter>

CameraView::CameraView(QWidget* parent) : QWidget(parent) {
    m_camera.setWidth(320);
    m_camera.setHeight(240);
    m_camera.setFormat(raspicam::RASPICAM_FORMAT_RGB);

    std::cout << "camera open" << std::endl;

    if(!m_camera.open())
        std::cerr << "Error opening camera" << std::endl;

    std::cout << "sleep" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(3)); // TODO: replace with a timer!

    m_data.resize(320*240*3);

    std::cout << "camera grab" << std::endl;

    m_camera.grab(); // blocking, TODO: change to a callback!

    std::cout << "camera retrieve" << std::endl;

    m_camera.retrieve(m_data.data(), raspicam::RASPICAM_FORMAT_RGB);

    std::cout << "qimage" << std::endl;

    m_image = QImage(m_data.data(), 320, 240, QImage::Format_RGB888);

    std::cout << "all done!" << std::endl;
}

void CameraView::paintEvent(QPaintEvent* e) {
    QPainter painter(this);

    painter.drawImage(QRect(QPoint(0,0),size()), m_image);
}
