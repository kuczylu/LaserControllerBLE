# LaserControllerBLE

LaserControllorBLE is a program designed to enable off-the-shelf laser range finders to be controlled with
Bluetooth Low Energy. When run in conjunction with a compatible mobile app, the laser can be controlled through the
interface on your smartphone.

This version of LaserControllerBLE is intended to be compiled with the Arduino IDE, and run on an 
AdaFruit Bluefruit Feather 32u4 board.

The specific laser module can be easily swapped out by including a ".h" file containing the new laser module's command set.

At its core, LaserControllerBLE handles the sending and recieving of commands via Bluetooth Low Energy with a 
connected device, while passing instructions to and reading output from a laser range finder connected via hardware serial.

