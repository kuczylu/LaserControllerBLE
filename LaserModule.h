#ifndef LASER_MODULE_H
#define LASER_MODULE_H


class LaserModule {
  public:
    LaserModule();
    bool initialize();
    bool openLaser();
    bool closeLaser();
    bool measure(double& distance);

  private:
    const unsigned int waitIntervalMs = 2;
    const unsigned int waitCountMax = 500;
    void sendCommand(const unsigned char* command, const unsigned int commandSize);
    bool receiveResult(const unsigned int buffSize, unsigned char* buff);
    bool verifyResult(const unsigned char* buff0, const unsigned char* buff1, const unsigned int buffSize);
};


#endif
