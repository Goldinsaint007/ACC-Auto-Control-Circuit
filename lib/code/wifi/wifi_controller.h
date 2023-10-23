#ifndef WIFI_CONTROLLER_H
#define WIFI_CONTROLLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <eeprom_config.h>

static constexpr int PORT               = 8181;         // network port
static constexpr int CONN_TIMEOUT       = 10;           // connect timeout
static constexpr unsigned long INTERVAL = 1000;         // Check for new connections every 1 second
static constexpr int MAX_CLIENTS        = 2;            // connect timeout
static constexpr int MAX_SENSORS        = 3;            // connect timeout
static constexpr char *AP_SSID          = "ACC_CLR";    // access point SSID
static constexpr char *AP_PASS          = "#acc1234";   // access point password

class WiFiSettings {
public:
    IPAddress ip;
    IPAddress subnetMask;
    IPAddress gateway;
    IPAddress dns;
    int port;

    WiFiSettings() {};

    void retrieveFromNetwork() {
        ip = WiFi.localIP();
        subnetMask = WiFi.subnetMask();
        gateway = WiFi.gatewayIP();
        dns = WiFi.dnsIP();
    }
};

class WiFiCredentials {
public:
    String ssid;
    String password;

    WiFiCredentials() {};
    WiFiCredentials(String _ssid, String _pass) : ssid(_ssid), password(_pass) {};
    
    void saveToEEPROM() {
        if(EEPROM.begin(EEPROM_SIZE)) {
            EEPROM.put(SSID_ADDRESS, ssid.c_str());
            EEPROM.put(PASS_ADDRESS, password.c_str());
            if(!EEPROM.commit()) {
                Serial.println("EEPROM commit error - saveToEEPROM()");
            }

            EEPROM.end();
        } else {
            Serial.println("EEPROM begin error - saveToEEPROM()");
        }
    }

    void loadFromEEPROM() {
        if(EEPROM.begin(EEPROM_SIZE)) {
            char storedSSID[SSID_LENGTH + 1];
            char storedPassword[PASS_LENGTH + 1];
            EEPROM.get(SSID_ADDRESS, storedSSID);
            EEPROM.get(PASS_ADDRESS, storedPassword);
            EEPROM.end();

            ssid = storedSSID;
            password = storedPassword;
        } else {
            Serial.println("EEPROM begin error - saveToEEPROM()");
        }
    }

    bool isStoredInEEPROM() {
        if(EEPROM.begin(EEPROM_SIZE)) {
            char storedSSID[SSID_LENGTH + 1];
            char storedPassword[PASS_LENGTH + 1];
            EEPROM.get(SSID_ADDRESS, storedSSID);
            EEPROM.get(PASS_ADDRESS, storedPassword);
            EEPROM.end();
            return (strlen(storedSSID) > 0 && strlen(storedPassword) > 0);
        } else {
            Serial.println("EEPROM begin error - clearEEPROM()");
            return false;
        }
    }

    void clearEEPROM() {
        if(EEPROM.begin(EEPROM_SIZE)) {
            for (int i = 0; i < EEPROM_SIZE; ++i) {
                EEPROM.write(i, 0);
            }

            if(!EEPROM.commit()) {
                Serial.println("EEPROM commit error - clearEEPROM()");
            }

            EEPROM.end();
        } else {
            Serial.println("EEPROM begin error - clearEEPROM()");
        }
    }
};

class TCPClient {
public:
    bool connected = false;
    IPAddress ip;
    int port;
    WiFiClient client;

    TCPClient() {};
    TCPClient(IPAddress _ip, int _port) : ip(_ip), port(_port) {};
};

class wifi_controller {
private:
    WiFiSettings settings = WiFiSettings();
    WiFiCredentials credentials = WiFiCredentials();
    TCPClient clients[MAX_CLIENTS];
    WiFiServer server;
    WiFiUDP udp;
    
    // Broadcasting variables
    unsigned long previousBroadcastTime = 0;
    bool broadcastSent = false;
    const unsigned long broadcastInterval = 5000; // Broadcast every 5 seconds

public:
    bool initialize();
    bool connect();
    void handleIncomingConnections();
    void listenForMessages();
    void checkConnectionStatus();
    void sendDiscoveryResponse(const IPAddress& clientIP, int clientPort);
    void sendBroadcastMessage(const String& message);
    void handleBroadcasting();
    void factoryReset();

    wifi_controller(/* args */);
};

#endif