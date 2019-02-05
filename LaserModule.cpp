#define USE_HI_AT

//#include <HardwareSerial.h>
#include <Arduino.h>
#include "LaserModule.h"
#ifdef USE_HI_AT
#include "HiAt.h"
#endif


LaserModule::LaserModule()
{
  
}


bool LaserModule::initialize()
{
  Serial1.begin(BAUD_RATE);

  unsigned int waitCount = 0;
  while(!Serial1 && (waitCount < waitCountMax)) {
      delay(waitIntervalMs);
      waitCount++;
  }

  return (waitCount < waitCountMax);
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
    if(verifyResult(resultBuffer, OPEN_RET_VAL, OPEN_RET_VAL_SIZE))
      return true;

  return false;
}


bool LaserModule::closeLaser() 
{
  sendCommand(CLOSE, CLOSE_SIZE);

  unsigned char resultBuffer[CLOSE_RET_VAL_SIZE] = {};
  
  if(receiveResult(CLOSE_RET_VAL_SIZE, resultBuffer))
    if(verifyResult(resultBuffer, CLOSE_RET_VAL, CLOSE_RET_VAL_SIZE))
      return true;

  return false;
}


bool LaserModule::measure(double& distance) 
{
  sendCommand(MEASURE, MEASURE_SIZE);

  unsigned char resultBuffer[MEASURE_RET_VAL_SIZE] = {};
  
  if(receiveResult(MEASURE_RET_VAL_SIZE, resultBuffer))
  {
    distance = getDistance(resultBuffer);
    return true;
  }

  return false;
}


bool LaserModule::receiveResult(const unsigned int buffSize, unsigned char* buff)
{
  bool hasResult = false;
  unsigned int waitCount = 0;
  unsigned int valCount = 0;

  while(!hasResult && (waitCount < waitCountMax))
  {
    if(Serial1.available() > 0)
    {
      int val = Serial1.read();

      while((val != -1) && (valCount < buffSize))
      {
        buff[valCount] = (unsigned char)val;
        val = Serial1.read();
        valCount++;
      }

      if(valCount == buffSize)
        hasResult = true;
    }

    delay(waitIntervalMs);
    waitCount++;
  }

  return hasResult;
}


bool LaserModule::verifyResult(const unsigned char* buff0, const unsigned char* buff1, const unsigned int buffSize)
{
  // check buff sizes are equal!! use template?
  
  for(unsigned int iVal = 0; iVal < buffSize; iVal++)
  {
      if(buff0[iVal] != buff1[iVal])
        return false;
  }

  return true;
}
