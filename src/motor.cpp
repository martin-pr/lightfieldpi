#include "motor.h"

#include <thread>
#include <unistd.h>

#include <wiringPi.h>

Motor::Motor(int enable_pin, int step_pin, int dir_pin, int endstop, bool dir_invert) : m_enablePin(enable_pin), m_stepPin(step_pin), m_dirPin(dir_pin), m_endstopPin(endstop), m_invert(dir_invert), m_position(1000) {
    pinMode(m_enablePin, OUTPUT);
    pinMode(m_stepPin, OUTPUT);
    pinMode(m_dirPin, OUTPUT);

    pinMode(m_endstopPin, INPUT);
    pullUpDnControl(m_endstopPin, PUD_UP);

    digitalWrite(m_enablePin, HIGH);
}

void Motor::move(int steps) {
    const bool dir = (steps > 0) ^ m_invert;

    digitalWrite(m_dirPin, dir ? HIGH : LOW);
    digitalWrite(m_stepPin, HIGH);
    delay(50);

    digitalWrite(m_enablePin, LOW);

    for(int a=0;a<abs(steps); ++a) {
        // break on endstop
        if(digitalRead(m_endstopPin) == LOW)
            break;

        if(m_position < 0 || (steps < 0 && m_position == 0))
            break;

        digitalWrite(m_stepPin, LOW);
        usleep(200);
        digitalWrite(m_stepPin, HIGH);
        usleep(200);

        // update position
        m_position += steps > 0 ? 1 : -1;

        if(m_position <= 0)
            break;
    }

    digitalWrite(m_enablePin, HIGH);
}

void Motor::home() {
    const bool dir = m_invert;

    digitalWrite(m_dirPin, dir ? HIGH : LOW);
    digitalWrite(m_stepPin, HIGH);
    delay(50);

    digitalWrite(m_enablePin, LOW);

    while(true) {
        // break on endstop
        if(digitalRead(m_endstopPin) == LOW)
            break;

        digitalWrite(m_stepPin, LOW);
        usleep(500);
        digitalWrite(m_stepPin, HIGH);
        usleep(500);
    }

    usleep(5000);

    digitalWrite(m_dirPin, !dir ? HIGH : LOW);
    for(int a=0;a<100;++a) {
        digitalWrite(m_stepPin, LOW);
        usleep(1000);
        digitalWrite(m_stepPin, HIGH);
        usleep(1000);
    }

    digitalWrite(m_enablePin, HIGH);

    // set the zero position
    m_position = 0;
}
