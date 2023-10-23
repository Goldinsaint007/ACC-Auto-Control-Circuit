#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>

enum LED_PIN {
    RED = 1,
    YELLOW = 2,
    GREEN = 3
};

class led_controller {
  public:
    int RED;
    int YELLOW;
    int GREEN;

    led_controller(int R, int Y, int G) : RED(R), YELLOW(Y), GREEN(G) {};

    void boot();
    void toogle(LED_PIN pin, bool on);
};

#endif