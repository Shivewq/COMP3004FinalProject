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
    void turnOn();
    void turnOff();
    void plugIn();
    int geneateDataPoint();
    bool isOn(){return onStatus;}
private:
    Battery* charge;
    bool onStatus;

public slots:

signals:
    void statusChange(bool);

};

#endif // DEVICE_H
