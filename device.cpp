#include "device.h"

Device::Device(): charge(new Battery())
{
}
void Device::turnOn(){charge->startTimer();}
void Device::turnOff(){}
