#include "BLEServer_generic.h"

#define BUFFER_SIZE 20

// We start by declaring our server class, we give it a name and a pin to show status
GenericBLEServer s("BatteryMonitor", LED_BUILTIN);

// Now we must declare services and characteristics. A service can have multiple characteristics.
//Like a device can have multiple services.
BLEService batteryService("1101");
BLEStringCharacteristic batteryLevelChar("2101", BLERead | BLENotify, BUFFER_SIZE);

char buf[BUFFER_SIZE];

void setup() {
    Serial.begin(9600);
    while (!Serial); // Wait for Serial to initialize

    Serial.println("Setting up BLE server...");
    // We use s.begin() to do the internal inicialization
    s.begin();
    // In our case we only add one characteristic to our service
    batteryService.addCharacteristic(batteryLevelChar);
    // We send that service to our device
    s.setService(batteryService);
    // And when we are done sending all services we can advertise
    //(showing ourselves to other Bluetooth devices)
    s.advertise();

    Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
    // We use it to wait for a connection
    s.connect();
    int battery = 0;
    //While we are connected
    while (s.isConnected()) {
        // We set the value of the characteristic to whatever we want, the other will see it
        sprintf(buf, "%d", battery++);
        batteryLevelChar.writeValue(buf);
    }
}
