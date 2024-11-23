#ifndef DEVICE_H
#define DEVICE_H
#include "battery.h"

class Device
{
public:
    Device();
    Battery* getBattery(){return charge;}
    void turnOn();
    void turnOff();
    Battery* charge;
private:

};

#endif // DEVICE_H
