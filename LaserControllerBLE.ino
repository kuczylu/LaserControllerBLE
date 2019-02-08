
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

    
  unsigned long startTime = millis();
  
  if(bleInterface.getCommand(command))
  {
    bool hasSuccess = false;
    double data = -1.0;
    
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
        hasSuccess = laser.measure(data);
        break;
      }
      default:
      {
        //sendMessage("Command not recognized");
      }
    }


    String resultStr = "r";
    resultStr += command;
    resultStr += hasSuccess ? "S" : "F";
    resultStr += getStringFromData(data);
    resultStr += "t";

    unsigned long endTime = millis();
    resultStr += getStringFromTime(endTime - startTime);
    resultStr += "e";
    
    sendMessage(resultStr);
  }
}


void sendMessage(const String& message)
{
  bleInterface.sendMessage(message);
}


String getStringFromTime(unsigned long t)
{
  String str = "";
  unsigned long temp = t;

  if(t > 9999)
  {
    temp = 9999;
  }

  unsigned long num = 1000;
  while(temp < num)
  {
    str += "0";
    num /= 10;
  }
  
  str += temp;
  
  return str;
}



String getStringFromData(double data)
{
  String str = "";

  if(data >= 0.0)
  {
    int dataHigh = floor(data);
    int dataLow = floor(100.0 * data) - 100 * dataHigh;

    int num = 1000;
    while(dataHigh < num)
    {
      str += "0";
      num /= 10;
    }
  
    str += dataHigh;
    str += ".";
    str += dataLow;
  }
  else
  {
    str = "0000.00";
  }
  
  return str;
}
