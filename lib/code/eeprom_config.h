#ifndef EEPROM_CONFIG_H
#define EEPROM_CONFIG_H

static constexpr int EEPROM_SIZE     = 512;                         // size of the EEPROM memory
static constexpr int SSID_ADDRESS    = 0;                           // start address of the Wi-Fi SSID string
static constexpr int SSID_LENGTH     = 32;                          // maximum length of the Wi-Fi SSID string
static constexpr int PASS_ADDRESS    = SSID_ADDRESS + SSID_LENGTH;  // start address of the Wi-Fi password string
static constexpr int PASS_LENGTH     = 64;                          // maximum length of the Wi-Fi password string
static constexpr int INIT_ADDRESS    = PASS_ADDRESS + PASS_LENGTH;  // address to store the initialization state of Wi-Fi credentials

#endif