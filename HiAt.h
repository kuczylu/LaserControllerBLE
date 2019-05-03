#ifndef HI_AT_H
#define HI_AT_H
// partial command set for Hi-AT Technology 50 meter laser ranging industrial control module
  
const unsigned int BAUD_RATE = 19200;
//command and return value sizes in bytes
const unsigned int OPEN_SIZE = 1;
const unsigned int CLOSE_SIZE = 1;
const unsigned int MEASURE_SIZE = 1;
const unsigned int OPEN_RET_VAL_SIZE = 7;
const unsigned int CLOSE_RET_VAL_SIZE = 7;
const unsigned int MEASURE_RET_VAL_SIZE = 16;
const unsigned int MEASURE_DATA_START = 2;
//commands
const unsigned char OPEN[OPEN_SIZE] = {0x4F};
const unsigned char CLOSE[CLOSE_SIZE] = {0x43};
const unsigned char MEASURE[MEASURE_SIZE] = {0x46};
// return values
const unsigned char OPEN_RET_VAL[OPEN_RET_VAL_SIZE] = {0x4F, 0x2C, 0x4F, 0x4B, 0x21, 0x0D, 0x0A};
const unsigned char CLOSE_RET_VAL[CLOSE_RET_VAL_SIZE] = {0x43, 0x2C, 0x4F, 0x4B, 0x21, 0x0D, 0x0A};
const unsigned char MEASURE_RET_VAL[MEASURE_RET_VAL_SIZE] = {0x46, 0x3A, 0X00, 0x00, 0x2E, 0x00, 0x00,
                                                             0x00, 0x6D, 0x2C, 0x00, 0x00, 0x00, 0x00,
                                                             0x0D, 0x0A};


unsigned long getDistance(const unsigned char* measureBuff)
{
  // function for decoding the measure command return value into a usable distance
  // measurement (unsigned long), in millimeters
  
  unsigned long distance = 0; // in millimeters

  // currently does not support measurements >= 10 meters
  distance += 1000 * static_cast<unsigned long>(*(measureBuff + MEASURE_DATA_START + 1) - '0');
  distance += 100 * static_cast<unsigned long>(*(measureBuff + MEASURE_DATA_START + 3) - '0'); // skip the decimal
  distance += 10 * static_cast<unsigned long>(*(measureBuff + MEASURE_DATA_START + 4) - '0');
  distance += 1 * static_cast<unsigned long>(*(measureBuff + MEASURE_DATA_START + 5) - '0');
  
  return distance;
}


#endif
