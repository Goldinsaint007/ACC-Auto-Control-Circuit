#include "wifi\wifi_controller.h"

bool wifi_controller::initialize() {
    if (!credentials.isStoredInEEPROM()) {
        Serial.println("No SSID or Password found!");

        // Start the access point
        Serial.println("Starting Access Point...");
        WiFi.softAP(AP_SSID, AP_PASS);

        IPAddress apIP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(apIP);

        udp.begin(settings.port);

        return false; // Return false to indicate AP mode
    }

    // Network credentials found, proceed with connecting as a client
    credentials.loadFromEEPROM();
    Serial.print("SSID and Password found... -> " + credentials.ssid + " & " + credentials.password);

    delay(1000);

    if (connect()) {
        return true; // Return true to indicate client mode
    }

    return false;
}

bool wifi_controller::connect() {
    WiFi.begin(credentials.ssid.c_str(), credentials.password.c_str());

    if (WiFi.status() == WL_CONNECT_FAILED) {
        Serial.println("Wi-Fi failed to connect!");
        return false;
    }

    Serial.print("Wi-Fi connecting : ");
    int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
        if (count >= 20) {
            //credentials.clearEEPROM();
            esp_restart();
        }
        delay(1000);
        Serial.print(".");
        count++;
    }

    settings.retrieveFromNetwork();

    Serial.println();
    Serial.println("Connected to WiFi");
    Serial.println("IP address: " + WiFi.localIP().toString());

    server.begin();
    udp.begin(settings.port);

    return true;
}

void wifi_controller::handleIncomingConnections() {
    static unsigned long previousTime = 0;

    if (millis() - previousTime >= INTERVAL) {
        previousTime = millis();

        WiFiClient client = server.available();

        if (client) {
            // a new client has connected
            if (client.connected()) {
                IPAddress clientIP = client.remoteIP();
                int clientPort = client.remotePort();

                // Find an available slot in the clients array
                int emptySlot = -1;
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (!clients[i].connected) {
                        emptySlot = i;
                        break;
                    }
                }

                if (emptySlot != -1) {
                    // Store the client information in the available slot
                    clients[emptySlot].client = client;
                    clients[emptySlot].connected = true;
                    clients[emptySlot].ip = clientIP;
                    clients[emptySlot].port = clientPort;
                    Serial.println("Client connected from " + clientIP.toString() + ":" + String(clientPort));
                    Serial.println("Client stored in slot " + String(emptySlot));
                } else {
                    // No available slots, reject the connection
                    Serial.println("Maximum number of clients reached. Rejecting connection from " + clientIP.toString() + ":" + String(clientPort));
                    client.stop();
                }
            }
        }
    }
}

void wifi_controller::checkConnectionStatus() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].connected) {
            if (!clients[i].client.connected()) {
                // Client disconnected
                Serial.println("Client at slot " + String(i) + " disconnected");
                clients[i].connected = false;
                // Perform any additional cleanup or handling for the disconnected client
            }
        }
    }
}

void wifi_controller::listenForMessages() {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].connected) {
            WiFiClient& client = clients[i].client;

            // Check if there is incoming data available from the client
            if (client.available()) {
                // Read the incoming data
                String message = client.readStringUntil('\n');
                // Process the received message
                Serial.println("Received message from client " + String(i) + ": " + message);
                // Perform any necessary actions based on the received message
            }
        }
    }
}

void wifi_controller::sendDiscoveryResponse(const IPAddress& clientIP, int clientPort) {
    String response = "ESP Device IP: " + WiFi.localIP().toString();
    WiFiClient responseClient;
    responseClient.connect(clientIP, clientPort);
    responseClient.println(response);
    responseClient.stop();
}

void wifi_controller::sendBroadcastMessage(const String& message) {
    IPAddress broadcastIP = WiFi.localIP();
    broadcastIP[3] = 255;
    udp.beginPacket(broadcastIP, settings.port);
    udp.print(message);
    udp.endPacket();
}

void wifi_controller::handleBroadcasting() {
    if (millis() - previousBroadcastTime >= broadcastInterval) {
        previousBroadcastTime = millis();
        sendBroadcastMessage("Discover ESP Device"); // Replace the message with your desired broadcast message
    }

    if (udp.parsePacket()) {
        IPAddress remoteIP = udp.remoteIP();
        int remotePort = udp.remotePort();
        String message = udp.readStringUntil('\n');

        // Process the received message
        if (message == "ESP Device Request") {
            sendDiscoveryResponse(remoteIP, remotePort);
        } else if (message == "Set SSID and Password") {
            // Handle other message types if needed
        } else {
            // Handle other message types if needed
        }
    }
}

void wifi_controller::factoryReset() {
    credentials.clearEEPROM();
}

wifi_controller::wifi_controller() {
}
