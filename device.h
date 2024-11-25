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
    void plugIn();
    int geneateDataPoint();
    bool isOn(){return onStatus;}
private:
    Battery* charge;
    bool onStatus;

public slots:

};

#endif // DEVICE_H
