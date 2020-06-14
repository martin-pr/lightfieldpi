#pragma once

#include <QWidget>

class CameraView : public QWidget {
    public:
        CameraView(QWidget* parent = nullptr);

        void showImage(QImage img);

    protected:
        virtual void paintEvent(QPaintEvent* e) override;

    private:
        QImage m_image;
};
