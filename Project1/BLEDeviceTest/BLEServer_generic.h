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
    void begin(); // New method for deferred initialization
    void setService(BLEService s);
    void advertise();
    int isConnected();
    void connect();
};

#endif
