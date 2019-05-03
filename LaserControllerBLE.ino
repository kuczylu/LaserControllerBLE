// LaserControllerBLE is designed to run on an AdaFruit BlureFruit Feather board, with a harware serial connection
// to a laser range finder. Once connected to a Bluetoth device, LaserControllerBLE listens for incoming Bluetooth
// commands, processes them, and sends relavant instructions to the laser via a hardware serial connection.
// Measured distances along with other relevant information is reported to the connected device via Bluetooth LE.


#include "BleInterface.h"
#include "LaserModule.h"

// create the ble interface and laser module objects, define the command set
BleInterface bleInterface;
LaserModule laser;
const char commandOpen = 'O';
const char commandClose = 'C';
const char commandMeasure = 'M';


void setup() {

  // pin 13 is tied to the on board LED, configure it for output, for debugging
  pinMode(13, OUTPUT);

  // wait for the bletooth interface to initialize, and connect to a device
  if (!bleInterface.initialize())
  {
    // if the bluetooth failed to initialize, blink the led forever
    while (1)
      blinkLed();
  }

  // if successful, send a message
  delay(500);
  sendMessage("BLE Interface connected..");

  //initialize the laser module
  if (laser.initialize())
  {
    delay(500);
    sendMessage("Laser serial connected..");
  }
  else
  {
    delay(500);
    sendMessage("Laser failed to initialize");
  }
}


void loop() {

  char command = 0x00;
  unsigned long startTime = millis();

  // if a command is found, process the command, and send it to the laser
  if (bleInterface.getCommand(command))
  {
    bool hasSuccess = false;
    unsigned long data = 0;

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
          break;
        }
    }

    // add the results to the outgoing packet
    // packet will be of the format: 0123333333455556 where:
    // 0 - "r"
    // 1 - "S" or "F"
    // 2 - "O", "C", or "M", or other command character
    // 3333333 - where "XXXXXXX" is the "0"-padded distance measurement in millimeters
    // 4 - "t"
    // 5555 - where "XXXX" is the "0"-padded time measurement in milliseconds
    // 6 - "e"

    String resultStr = "r";
    resultStr += hasSuccess ? "S" : "F";
    resultStr += command;
    resultStr += getStringFromData(data, 7);
    resultStr += "t";

    unsigned long endTime = millis();
    resultStr += getStringFromData(endTime - startTime, 4);
    resultStr += "e";

    // send the information
    sendMessage(resultStr);
  }
}


void sendMessage(const String& message)
{
  bleInterface.sendMessage(message);
}


String getStringFromData(unsigned long data, unsigned long maxDigits)
{
  // convert data (unsigned long) into a zero padded string for transimission
  // if the data value is greater than the available space, set it to the max (all nines)
  unsigned long maxValue = pow(10, maxDigits) - 1;
  unsigned long dataValue = (data > maxValue) ? maxValue : data;
  String dataStr = "";

  // pad the string with zeros
  unsigned long val = pow(10, maxDigits - 1);
  while (dataValue < val)
  {
    dataStr += "0";
    val /= 10;
  }

  // add the remaining digits
  dataStr += dataValue;

  return dataStr;
}


void blinkLed()
{
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
}
