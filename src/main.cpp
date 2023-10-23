#include <Arduino.h>
#include <wifi\wifi_controller.h>
#include <led\led_controller.h>

wifi_controller wifi;
led_controller led(19, 18, 17);
int RST_BTN = 27;

bool normalMode;

void setup() {
    delay(1000);
    Serial.begin(115200);
    delay(1000);
    pinMode(GPIO_NUM_17, OUTPUT);
    pinMode(GPIO_NUM_18, OUTPUT);
    pinMode(GPIO_NUM_19, OUTPUT);
    pinMode(GPIO_NUM_27, INPUT);
    delay(1000);
    Serial.println();

    led.boot();

    normalMode = wifi.initialize();

    delay(3000);

    if(normalMode) {
        led.toogle(YELLOW, true);
    } else {
        led.toogle(GREEN, true);
    }
}

void loop() {
    int btn_chk = digitalRead(RST_BTN);
    proccessButton(btn_chk);

    if(normalMode) {
        wifi.handleIncomingConnections();
        wifi.handleBroadcasting();
        wifi.listenForMessages();
        wifi.checkConnectionStatus();
    }
    
    // Add any other necessary code or logic here
}

void proccessButton(int value) {
    //
}
