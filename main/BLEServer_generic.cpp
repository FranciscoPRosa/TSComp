#include "BLEServer_generic.h"

// Constructor: Stores initial values but does not perform hardware operations
GenericBLEServer::GenericBLEServer(const String& name, int statusPin)
    : statusPin(statusPin), localName(name) {}

// New `begin` method: Performs actual initialization
void GenericBLEServer::begin() {
    pinMode(statusPin, OUTPUT);

    if (!BLE.begin()) {
        if (Serial) Serial.println("[ERROR] Starting BLE failed!");
        while (1) {
          if (statusPin != -1) {
              digitalWrite(statusPin, !digitalRead(statusPin)); // Blink LED
          }
          delay(500);
        }
    }
    BLE.setLocalName(localName.c_str());
    Serial.println("BLE initialized successfully.");
}

// Set BLE Service
void GenericBLEServer::setService(BLEService s) {
    BLE.setAdvertisedService(s);
    BLE.addService(s);
}

// Start Advertising
void GenericBLEServer::advertise() {
    BLE.advertise();
    if (Serial) Serial.println("Bluetooth device active, waiting for connections...");
}

// Connect to a Central Device
void GenericBLEServer::connect() {
    central = BLE.central();
    if (central) {
        digitalWrite(statusPin, HIGH); // Turn on LED to indicate connection
        Serial.print("Connected to central: ");
        Serial.println(central.address());
    }
}

// Check Connection Status
int GenericBLEServer::isConnected() {
    if (central && central.connected()) {
        return 1;
    }
    digitalWrite(statusPin, LOW); // Turn off LED if disconnected
    return 0;
}
