#include "battery.h"

Battery::Battery(QObject* parent):QObject(parent), battery(100), batteryTimer(new QTimer())
{

}
//connects the battery timeout to update the power and starts the timer
void Battery::startTimer(){
    connect(batteryTimer,&QTimer::timeout,this,&Battery::updateBattery);
     batteryTimer->start(2000); //2 second timer
}
void Battery::updateBattery(){
    if(battery != 0){
        battery -= 5;
        emit editBattery(battery);
    }
    //when battery is empty
    else {}
    //if the battery reaches 20% warn user
    if(battery == 20){
        emit lowBatteryWarning();
    }
}
