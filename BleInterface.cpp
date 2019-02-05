#define FACTORYRESET_ENABLE         1
#define USE_BLE_DEVICE_OUTPUT       1


#include "BleInterface.h"
#include "BluefruitConfig.h"


BleInterface::BleInterface() :
  ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST)
{
  
}


bool BleInterface::initialize()
{  
  if(!ble.begin(VERBOSE_MODE))
    return false;


  if(FACTORYRESET_ENABLE)
  {
    if(!ble.factoryReset())
      return false;
  }

  ble.echo(false);
  //ble.info();
  ble.verbose(false);

  while (!ble.isConnected()) {
      delay(500);
  }

  return true;
}


bool BleInterface::getCommand(char& command)
{
  ble.println("AT+BLEUARTRX");
  ble.readline();
  if(strcmp(ble.buffer, "OK") != 0) 
  {
    command = ble.buffer[0];

    return true;
  }

  return false;
}


void BleInterface::sendMessage(const String& message)
{
  ble.print("AT+BLEUARTTX=");
  ble.println(message);
}
