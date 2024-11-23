#include "device.h"
#include <random>
Device::Device(): charge(new Battery())
{
}
void Device::turnOn(){charge->startTimer();}
void Device::turnOff(){}
//generates a datapoint.
//45-70 is normal. < 45 is low functionality, > 70 is high functionality
int Device::geneateDataPoint(){
    std::random_device rd; // Non-deterministic random seed
    std::mt19937 gen(rd()); // Mersenne Twister engine
    //std::uniform_int_distribution<> dist(5, 160); // Distribution in range [min, max]. //before change
    std::uniform_int_distribution<> dist(5, 160);
    return dist(rd);
}
