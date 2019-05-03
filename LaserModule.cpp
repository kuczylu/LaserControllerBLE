#define USE_HI_AT // use the Hi-AT laser command set

#include <Arduino.h>
#include "LaserModule.h"
#ifdef USE_HI_AT
#include "HiAt.h" // import the Hi-AT commands and other module-specific information
#endif


LaserModule::LaserModule()
{
  
}


bool LaserModule::initialize()
{
  // begin the hardware serial connection at the baud rate specified by the laser command file
  Serial1.begin(BAUD_RATE);

  unsigned int waitTimeMs = 0;
  while(!Serial1 && (waitTimeMs < waitTimeMaxMs)) {
      delay(waitIntervalMs);
      waitTimeMs += waitIntervalMs;
  }

  // return false if the laser connection times out
  return (waitTimeMs < waitTimeMaxMs);
}


void LaserModule::sendCommand(const unsigned char* command, const unsigned int commandSize)
{
  for(unsigned int iVal = 0; iVal < commandSize; iVal++)
    Serial1.write(command[iVal]);
}


bool LaserModule::openLaser() 
{
  sendCommand(OPEN, OPEN_SIZE);

  unsigned char resultBuffer[OPEN_RET_VAL_SIZE] = {};
  
  if(receiveResult(OPEN_RET_VAL_SIZE, resultBuffer))
    return verifyResult(resultBuffer, OPEN_RET_VAL, OPEN_RET_VAL_SIZE);

  return false;
}


bool LaserModule::closeLaser() 
{
  sendCommand(CLOSE, CLOSE_SIZE);

  unsigned char resultBuffer[CLOSE_RET_VAL_SIZE] = {};
  
  if(receiveResult(CLOSE_RET_VAL_SIZE, resultBuffer))
    return verifyResult(resultBuffer, CLOSE_RET_VAL, CLOSE_RET_VAL_SIZE);

  return false;
}


bool LaserModule::measure(unsigned long& distance) 
{
  sendCommand(MEASURE, MEASURE_SIZE);

  unsigned char resultBuffer[MEASURE_RET_VAL_SIZE] = {};
  
  if(receiveResult(MEASURE_RET_VAL_SIZE, resultBuffer))
  {
    distance = getDistance(resultBuffer);
    
     // only verify up to the start of the data, as the data bits will be different for each measurement
    return verifyResult(resultBuffer, MEASURE_RET_VAL, MEASURE_DATA_START);
  }

  return false;
}


bool LaserModule::receiveResult(const unsigned int buffSize, unsigned char* buff)
{
  // listens for values coming over the hardware serial line, 
  // called after sending a command, to capture the result
  bool hasResult = false;
  unsigned int waitTimeMs = 0;
  unsigned int valCount = 0;

  // contiuously check the hardware serial for incoming values, 
  // time out after a given max wait time
  while(!hasResult && (waitTimeMs < waitTimeMaxMs))
  {
    // if a value is found, read the data until it's empty
    // or until the given buffer is full
    if(Serial1.available() > 0)
    {
      int val = Serial1.read();

      while((val != -1) && (valCount < buffSize))
      {
        buff[valCount] = (unsigned char)val;
        val = Serial1.read();
        valCount++;
      }

      // if the buffer is full, the result is valid
      if(valCount == buffSize)
        hasResult = true;
    }

    delay(waitIntervalMs);
    waitTimeMs += waitIntervalMs;
  }

  return hasResult;
}


bool LaserModule::verifyResult(const unsigned char* buff0, const unsigned char* buff1, const unsigned int buffSize)
{
  // verify that the values contained in the two buffers are identical
  for(unsigned int iVal = 0; iVal < buffSize; iVal++)
  {
      if(buff0[iVal] != buff1[iVal])
        return false;
  }

  return true;
}
