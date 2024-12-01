#include <ArduinoBLE.h>

#define TARGET_NAME "SensorSystemG4"
#define TARGET_SERVICE_UUID "19B10000-E8F2-537E-4F6C-D104768A1214"
#define TARGET_CHARACTERISTIC_UUID "19B10001-E8F2-537E-4F6C-D104768A1214"

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Begin BLE initialization
  if (!BLE.begin()) {
    Serial.println("Starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  Serial.println("Bluetooth® Low Energy Central - Distance Receiver");
  Serial.println("Make sure the peripheral device is on and broadcasting.");

  // Start scanning for peripherals
  BLE.scan();
}

void loop() {
  // Check if a peripheral is discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // Discovered a peripheral, print details
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    // Check if the peripheral is the target device
    if (peripheral.localName() == TARGET_NAME) {
      BLE.stopScan(); // Stop scanning
      monitor(peripheral); // Monitor the peripheral
      BLE.scan(); // Resume scanning if disconnected
    }
  }
}

void monitor(BLEDevice peripheral) {
  // Connect to the peripheral
  Serial.println("Connecting ...");
  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // Discover the target service
  Serial.println("Discovering service...");
  if (!peripheral.discoverService(TARGET_SERVICE_UUID)) {
    Serial.println("Service discovery failed!");
    peripheral.disconnect();
    return;
  }

  // Retrieve the characteristic for distance measurement
  BLECharacteristic distanceCharacteristic = peripheral.characteristic(TARGET_CHARACTERISTIC_UUID);

  // Check if the characteristic is valid and subscribable
  if (!distanceCharacteristic) {
    Serial.println("Characteristic not found!");
    peripheral.disconnect();
    return;
  } else if (!distanceCharacteristic.canSubscribe()) {
    Serial.println("Characteristic is not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!distanceCharacteristic.subscribe()) {
    Serial.println("Subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed -> Ready to receive data!");
  }

  // Loop while connected to receive and process data
  while (peripheral.connected()) {
    if (distanceCharacteristic.valueUpdated()) {
      const uint8_t* value = distanceCharacteristic.value();
      size_t length = distanceCharacteristic.valueLength();
      
      // Convert the received uint8_t* to a String
      String receivedDistance((const char*)value, length);
      Serial.println("Received Distance: " + receivedDistance);
    }
  }

  Serial.println("Peripheral disconnected!");
}
