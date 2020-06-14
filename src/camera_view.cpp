#include "camera_view.h"

#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>

#include <QPainter>

CameraView::CameraView(QWidget* parent) : QWidget(parent) {
}

void CameraView::paintEvent(QPaintEvent* e) {
    if(!m_image.isNull() && m_image.width() > 0 && m_image.height() > 0) {
        // figure out the size
        const float aspect = (float)width() / (float)height();
        const float imgAspect = (float)m_image.width() / (float)m_image.height();

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

    else
        QWidget::paintEvent(e);
}

void CameraView::showImage(QImage img) {
    m_image = img;
    update();
}
