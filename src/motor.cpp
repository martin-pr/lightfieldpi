#include "motor.h"

#include <thread>
#include <unistd.h>

#include <wiringPi.h>

Motor::Motor(int enable_pin, int step_pin, int dir_pin, bool dir_invert) : m_enablePin(enable_pin), m_stepPin(step_pin), m_dirPin(dir_pin), m_invert(dir_invert) {
    pinMode(m_enablePin, OUTPUT);
    pinMode(m_stepPin, OUTPUT);
    pinMode(m_dirPin, OUTPUT);

    digitalWrite(m_enablePin, HIGH);
}

void Motor::move(int steps) {
    const bool dir = (steps > 0) ^ m_invert;

    digitalWrite(m_dirPin, dir ? HIGH : LOW);
    digitalWrite(m_stepPin, HIGH);
    delay(50);

    digitalWrite(m_enablePin, LOW);

    for(int a=0;a<abs(steps); ++a) {
        digitalWrite(m_stepPin, LOW);
        usleep(200);
        digitalWrite(m_stepPin, HIGH);
        usleep(200);
    }

    digitalWrite(m_enablePin, HIGH);
}

