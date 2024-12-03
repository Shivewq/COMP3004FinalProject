#include "battery.h"

Battery::Battery(QObject* parent):QObject(parent), battery(100), batteryTimer(new QTimer())
{

}


//starts a new battery connection every time the device is turned on
void Battery::startTimer(){
    disconnect(batteryTimer, &QTimer::timeout, this, &Battery::updateBattery);
    connect(batteryTimer,&QTimer::timeout,this,&Battery::updateBattery);
     batteryTimer->start(5000); //5 second timer
}

//stops battery timer when device is off
void Battery::stopTimer(){
    batteryTimer->stop();
}

//sets battery to 100
void Battery::chargeBattery(){
    battery = 100;

    emit editBattery(battery);
}

//decreased battery periodically
//signals correct battery messages
void Battery::updateBattery(){
    if(battery != 0){
        battery -= 5;
        emit editBattery(battery);
    }
    //when battery is empty
    else {
       emit batteryOut();
    }
    //if the battery reaches 20% warn user
    if(battery == 20){
        emit lowBatteryWarning();
    }
}
