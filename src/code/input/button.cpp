#include "input/button.h"

buttonStatus Button::check() {
    int val = digitalRead(pin);

    if (val == LOW && isPressed == false) {
        isPressed = true;
        startTime = millis();
        return PRESSED;
    }
    
    if (val == LOW && isPressed == true) {
        unsigned long int currTime = millis();

        if (currTime - startTime >= duration) {
            startTime = millis();
            return HELD_DOWN;
        }
    }
    
    if (val == HIGH && isPressed == true) {
        isPressed = false;
        return RELEASED;
    }

    return NONE;
}