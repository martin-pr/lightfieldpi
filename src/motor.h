#pragma once

class Motor {
    public:
        Motor(int enable_pin, int step_pin, int dir_pin, bool dir_invert = false);

        void move(int steps);

    private:
        int m_enablePin, m_stepPin, m_dirPin;
        bool m_invert;
};
