#ifndef BATTERY_H
#define BATTERY_H
#include <QTimer>
#include <QObject>
//if we are going to send a signal for when battery runs out needs to be a QObject
class Battery: public QObject{
     Q_OBJECT
public:
    explicit Battery(QObject* parent = nullptr);
    QTimer* getBattery(){return batteryTimer;}
    void startTimer();
private:
    int battery;
    QTimer* batteryTimer;
public slots:
   void updateBattery();
signals:
   void editBattery(int value);
};

#endif // BATTERY_H
