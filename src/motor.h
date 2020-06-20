#pragma once

class Motor {
    public:
        Motor(int enable_pin, int step_pin, int dir_pin);

        void move(int steps);

        void setMoveInvert(bool invert);

    private:
        int m_enablePin, m_stepPin, m_dirPin;
        bool m_invert;
};
