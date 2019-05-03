#ifndef LASER_MODULE_H
#define LASER_MODULE_H


class LaserModule {
  public:
    LaserModule();
    bool initialize();
    bool openLaser();
    bool closeLaser();
    bool measure(unsigned long& distance);

  private:
    const unsigned int waitIntervalMs = 5; // mlliseconds
    const unsigned int waitTimeMaxMs = 2500; // milliseconds
    void sendCommand(const unsigned char* command, const unsigned int commandSize);
    bool receiveResult(const unsigned int buffSize, unsigned char* buff);
    bool verifyResult(const unsigned char* buff0, const unsigned char* buff1, const unsigned int buffSize);
};


#endif
