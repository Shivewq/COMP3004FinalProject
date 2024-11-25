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
    int geneateDataPoint();
private:
    Battery* charge;

public slots:

};

#endif // DEVICE_H
