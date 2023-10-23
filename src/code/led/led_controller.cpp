#include "led\led_controller.h"

void led_controller::boot() {
    digitalWrite(17, HIGH);
    delay(1000);

    digitalWrite(17, LOW);
    digitalWrite(18, HIGH);
    delay(1000);

    digitalWrite(18, LOW);
    digitalWrite(19, HIGH);
    delay(1000);

    digitalWrite(19, LOW);
    delay(1000);

    digitalWrite(17, HIGH);
    digitalWrite(18, HIGH);
    digitalWrite(19, HIGH);
    delay(500);

    digitalWrite(17, LOW);
    digitalWrite(18, LOW);
    digitalWrite(19, LOW);
    delay(500);

    digitalWrite(17, HIGH);
    digitalWrite(18, HIGH);
    digitalWrite(19, HIGH);
    delay(500);

    digitalWrite(17, LOW);
    digitalWrite(18, LOW);
    digitalWrite(19, LOW);
}

void led_controller::toogle(LED_PIN pin, bool on) {
  switch (pin) {
    case 1: // RED
      digitalWrite(RED, on);
      break;

    case 2: // YELLOW
      digitalWrite(YELLOW, on);
      break;

    case 3: // GREEN
      digitalWrite(GREEN, on);
      break;
    
    default:
      break;
  }
}