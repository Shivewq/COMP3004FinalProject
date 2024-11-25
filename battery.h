#ifndef BATTERY_H
#define BATTERY_H
#include <QTimer>
#include <QObject>
//if we are going to send a signal for when battery runs out needs to be a QObject


//basically calling device->turnOn() starts the timer and after every 2 seconds it lowers the battery
class Battery: public QObject{
     Q_OBJECT
public:
    explicit Battery(QObject* parent = nullptr);
    QTimer* getBattery(){return batteryTimer;}
    void startTimer();
    void stopTimer();
    void chargeBattery();
    int getCharge(){return battery;}
private:
    int battery;
    QTimer* batteryTimer;
public slots:
   void updateBattery();
signals:
   void editBattery(int value);
   void lowBatteryWarning();
};

#endif // BATTERY_H
