#include "device.h"

Device::Device(QObject* parent):QObject(parent), charge(new Battery())
{
}
void Device::turnOn(){
    if(charge->getCharge() == 0) return; //if battery is dead don't turn on
    charge->startTimer();
    onStatus = true;
    emit statusChange(onStatus);
}
void Device::turnOff(){
    charge->stopTimer();
    onStatus = false;
    emit statusChange(onStatus);


}
void Device::plugIn(){charge->chargeBattery();}
/*
 * Generates a random datapoint.
 * 45-70 is normal. < 45 is low functionality, > 70 is high functionality
 * 60% chance of a normal reading
 * 40% chance of an abnormal reading (either a high or low reading)
 * If we want the reading to be more/less likely of being normal/abnormal only need to change the if p statement.
*/
int Device::geneateDataPoint(){
    std::random_device rd; // Non-deterministic random seed
    std::mt19937 gen(rd()); // Mersenne Twister engine
    //std::uniform_int_distribution<> dist(5, 160); // Distribution in range [min, max]. //before change
    std::uniform_real_distribution<> dist(0.0,1.0); //getting probability
    double p = dist(gen);
    if(p < 0.6){ //60% chance of the reading being normal
        std::uniform_int_distribution<> range45to70(45,70);
        return range45to70(gen);
    }
    else{ //40% chance of it being either a low or high reading
        if(dist(gen) < 0.5){
            //20% chance of it being a low reading
            std::uniform_int_distribution<> range5to45(5,45);
            return range5to45(gen);
        }
        else{ //20% chance of being a high reading
            std::uniform_int_distribution<> range70to160(70,160);
            return range70to160(gen);
        }
    }
}
