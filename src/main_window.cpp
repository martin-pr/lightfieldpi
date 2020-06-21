#include "main_window.h"

#include <sstream>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <iomanip>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QThreadPool>
#include <QProgressDialog>
#include <QCoreApplication>

#include "config.h"
#include <wiringPi.h>

namespace {
    static const std::vector<int> s_multipliers({{
        1, 2, 4, 8
    }});

    static const std::vector<int> s_steps({{
        3, 20, 50, 200
    }});

    QPushButton* makeButton(QString label, std::function<void()> callback, int xsize = 40) {
        QPushButton* button = new QPushButton(label);
        button->setMinimumSize(QSize(0, 0));
        button->setMaximumSize(QSize(xsize, 30));
        QPushButton::connect(button, &QPushButton::pressed, callback);
        return button;
    }

    class ImageSaver : public QRunnable {
        public:
            ImageSaver(QImage img, const std::string& filename, int index, int& counter) : m_img(img), m_filename(filename), m_index(index), m_counter(counter) {
                setAutoDelete(true);
            }

            virtual void run() override {
                std::stringstream f;
                f << m_filename << std::setfill('0') << std::setw(3) << m_index << ".png";

                m_img.save(f.str().c_str());

                --m_counter;
            }

        private:
            QImage m_img;
            std::string m_filename;
            int m_index;

            int& m_counter;
    };
}

MainWindow::MainWindow() : QMainWindow(), m_motor1(M1_ENABLE, M1_STEP, M1_DIR, M1_ENDSTOP, M1_DIR_INVERT), m_motor2(M2_ENABLE, M2_STEP, M2_DIR, M2_ENDSTOP, M2_DIR_INVERT), m_continuous(true) {
    // setup camera
    connect(&m_camera, &Camera::imageReady, [this]() {
        if(m_continuous) {
            auto img = m_camera.capture();
            m_cameraView->showImage(img);
        }
    });

    // setup the widgets
    QWidget* central = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(central);
    layout->setContentsMargins(0,0,0,0);

    m_cameraView = new CameraView();
    layout->addWidget(m_cameraView, 4);

    QWidget* controls = new QWidget();
    QVBoxLayout* form = new QVBoxLayout(controls);

    QComboBox* resolution = new QComboBox();
    {
        auto baseResolution = Camera::baseResolution();
        for(auto mult: s_multipliers) {
            std::stringstream ss;
            ss << (baseResolution.first * (mult)) << " x " << (baseResolution.second * (mult));

            resolution->addItem(ss.str().c_str());
        }
    }

    connect(resolution, (void (QComboBox::*)(int))(&QComboBox::currentIndexChanged), [this](int val) {
        assert(val >= 0 && val < s_multipliers.size());
        m_camera.setResolutionMultiplier(s_multipliers[val]);
    });

    form->addWidget(new QLabel("Preview:"));
    form->addWidget(resolution);

    // motor moves
    QHBoxLayout* m1_moves = new QHBoxLayout();
    m1_moves->addWidget(makeButton("Home", [this]() { m_motor1.home(); }, 60));
    m1_moves->addWidget(makeButton("-500", [this]() { m_motor1.move(-500); }));
    m1_moves->addWidget(makeButton("-100", [this]() { m_motor1.move(-100); }));
    m1_moves->addWidget(makeButton("+100", [this]() { m_motor1.move(100); }));
    m1_moves->addWidget(makeButton("+500", [this]() { m_motor1.move(500); }));

    form->addWidget(new QLabel("Motor 1 moves:"));
    form->addLayout(m1_moves);

    QHBoxLayout* m2_moves = new QHBoxLayout();
    m2_moves->addWidget(makeButton("Home", [this]() { m_motor2.home(); }, 60));
    m2_moves->addWidget(makeButton("-500", [this]() { m_motor2.move(-500); }));
    m2_moves->addWidget(makeButton("-100", [this]() { m_motor2.move(-100); }));
    m2_moves->addWidget(makeButton("+100", [this]() { m_motor2.move(100); }));
    m2_moves->addWidget(makeButton("+500", [this]() { m_motor2.move(500); }));

    form->addWidget(new QLabel("Motor 2 moves:"));
    form->addLayout(m2_moves);

    // capture UI
    form->addWidget(new QLabel("Capture steps:"));
    QComboBox* steps = new QComboBox();
    {
        for(auto s: s_steps) {
            std::stringstream ss;
            ss << s;

            steps->addItem(ss.str().c_str());
        }
    }
    form->addWidget(steps);

    QPushButton* go = new QPushButton("Go!");
    QPushButton::connect(go, &QPushButton::pressed, [this, steps]() {
        time_t rawtime = time(0);
        struct tm * now;
        now = gmtime (&rawtime);

        std::string filename_base;
        {
            std::stringstream ss;
            ss << now->tm_year << "_" << std::setfill('0') << std::setw(2) << (now->tm_mon)+1 << "_" << std::setfill('0') << std::setw(2) << now->tm_mday << "_" << std::setfill('0') << std::setw(2) << now->tm_hour << "_" << std::setfill('0') << std::setw(2) << now->tm_min << "_" << std::setfill('0') << std::setw(2) << now->tm_sec << "_";

            filename_base = ss.str();
        }

        const Camera::State original = m_camera.state();

        const Camera::State manual = m_camera.manualState();
        m_camera.setState(manual);

        // delay(10000); // 10s delay for the camera to adjust!

        m_motor1.home();

        const int step = M1_RAIL_LENGTH / ((s_steps[steps->currentIndex()]-1));
        const int end = step * ((s_steps[steps->currentIndex()]-1));

        m_continuous = false;

        {
            std::vector<QRunnable*> tasks;

            int current = 0;
            int index = 0;

            int imageProgressCounter = 0;

            while(current < end) {
                delay(1000); // allow the camera to stabilise

                auto img = m_camera.capture();
                m_cameraView->showImage(img);
                repaint();

                {
                    int i = index++;
                    tasks.push_back(new ImageSaver(img.copy(), filename_base, i, imageProgressCounter));
                    ++imageProgressCounter;
                }

                current += step;
                m_motor1.move(step);
            }

            QThreadPool threadpool;
            threadpool.setExpiryTimeout(-1);
            // threadpool.setMaxThreadCount(std::max(1, QThread::idealThreadCount())); // at least 1 thread, but try to keep one free for the main loop

            for(auto& i : tasks)
                threadpool.start(i);

            {
                QProgressDialog progress("Saving...", "Cancel", 0, tasks.size());
                progress.show();

                while(imageProgressCounter > 0) {
                    progress.setValue(progress.maximum() - imageProgressCounter);

                    QCoreApplication::processEvents();

                    delay(500);
                }

                threadpool.waitForDone();
            }
        }

        m_continuous = true;

        m_camera.setState(original);
    });
    form->addWidget(go);

    // filler
    form->addWidget(new QWidget(), 1);

    layout->addWidget(controls, 1);

    setCentralWidget(central);
}
