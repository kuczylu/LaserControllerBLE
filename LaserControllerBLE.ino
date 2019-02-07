
#include "BleInterface.h"
#include "LaserModule.h"


BleInterface bleInterface;
LaserModule laser;
const char commandOpen = 'O';
const char commandClose = 'C';
const char commandMeasure = 'M';


void setup() {

  if(bleInterface.initialize())
  {
    delay(500);
    sendMessage("BLE Interface connected..");
  }

  if(laser.initialize())
  {
    delay(500);
    sendMessage("Laser serial connected..");
  }
}


void loop() {

  char command = 0x00;

  if(bleInterface.getCommand(command))
  {
    sendMessage(String(command) + " - rec");
    bool hasSuccess = false;
    double distance = -1.0;
    
    switch (command)
    {
      case commandOpen:
      {
        hasSuccess = laser.openLaser();
        break;
      }
      case commandClose:
      {
        hasSuccess = laser.closeLaser();
        break;
      }
      case commandMeasure:
      {
        hasSuccess = laser.measure(distance);
        sendMessage(String(distance));
        break;
      }
      default:
      {
        sendMessage("Command not recognized");
      }
    }
    
    String resultStr = "Command ";
    resultStr += hasSuccess ? "succeeded" : "failed";
    sendMessage(resultStr);
    
  }
}


void sendMessage(const String& message)
{
  bleInterface.sendMessage(message);
}
