#ifndef BLE_INTERFACE_H
#define BLE_INTERFACE_H


#include "Adafruit_BluefruitLE_SPI.h"


class BleInterface {
  public:
    BleInterface();
    bool initialize();
    bool getCommand(char& command);
    void sendMessage(const String& message);

  private:
    Adafruit_BluefruitLE_SPI ble;
};


#endif
