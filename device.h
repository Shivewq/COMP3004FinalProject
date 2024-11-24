#ifndef DEVICE_H
#define DEVICE_H
#include "battery.h"
#include <random>
class Device
{
public:
    Device();
    Battery* getBattery(){return charge;}
    void turnOn();
    void turnOff();
    Battery* charge;
    int geneateDataPoint();
private:

public slots:

};

#endif // DEVICE_H
