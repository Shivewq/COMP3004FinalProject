#ifndef DEVICE_H
#define DEVICE_H
#include "battery.h"
#include <random>
#include <QObject>
class Device: public QObject{
    Q_OBJECT

public:
    explicit Device(QObject* parent = nullptr);
    Battery* getBattery(){return charge;}

    //battery value functions
    void turnOn();
    void turnOff();
    void plugIn();

    int geneateDataPoint();
    bool isOn(){return onStatus;}

private:
    Battery* charge; //battery object
    bool onStatus; //whether the device is on or off

signals:
    void statusChange(bool);

};

#endif // DEVICE_H
