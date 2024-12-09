#ifndef BLESERVER_GEN_H
#define BLESERVER_GEN_H

#include <ArduinoBLE.h>

class GenericBLEServer {
private:
    int statusPin;
    String localName;
    BLEDevice central;

public:
    GenericBLEServer(const String& name, int statusPin);
    /**
     * @brief Begin the BLE server
     * 
     * This method initializes the BLE server and sets the local name
     * @warning This will cause a block until the BLE server is initialized
     * 
     * @param name The name of the BLE device
     * @param statusPin The pin to be used as status LED
     */
    void begin(); // New method for deferred initialization

    /**
     * @brief Adds a BLE Service to the BLE stack
     * 
     * @param s The BLEService object to be added
     */
    void setService(BLEService s);

    /**
     * @brief Start advertising the BLE device
     * 
     * MUST be called after at least on call to `setService` and before `connect`!
     */
    void advertise();

    /**
     * @brief Are we connected to any device?
     * 
     * @return 1 if connected, 0 otherwise
     */
    int isConnected();

    /**
     * @brief Connect to a Central Device
     * 
     * @warning This method will block until a central device is connected,
     * after a connection is established, the LED will turn on.
     */
    void connect();
};

#endif
