#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

enum buttonStatus {
    PRESSED,
    HELD_DOWN,
    RELEASED,
    NONE
};

class Button {
    private:
        uint8_t pin;
        boolean isPressed = false;

        // Timer vars for the button hold down function
        unsigned long int startTime;
        int duration = 500;
    public:
        Button(uint8_t _pin, int _duration = 500) : pin(_pin), duration(_duration) {
            pinMode(_pin, INPUT_PULLUP);
        };

        buttonStatus check();
};


#endif