#ifndef BATTERY_H
#define BATTERY_H
#include <QTimer>
#include <QObject>


class Battery: public QObject{
     Q_OBJECT

public:
    explicit Battery(QObject* parent = nullptr);
    //battery manipulation functions
    void startTimer();
    void stopTimer();
    void chargeBattery();

    //battery getters
    int getCharge(){return battery;}
     QTimer* getBattery(){return batteryTimer;}

private:
    int battery; //battery number value
    QTimer* batteryTimer;

public slots:
   void updateBattery();

signals:
   void editBattery(int value);
   void lowBatteryWarning();
   void batteryOut();
};

#endif // BATTERY_H
