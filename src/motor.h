#pragma once

class Motor {
    public:
        Motor(int enable_pin, int step_pin, int dir_pin, int endstop, bool dir_invert = false);

        void move(int steps);
        void home();

    private:
        int m_enablePin, m_stepPin, m_dirPin, m_endstopPin;
        bool m_invert;

        int m_position;
};
