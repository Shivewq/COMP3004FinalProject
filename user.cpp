#include "user.h"
#include "scan.h"
User::User(QObject* parent,QString name, int weight, int height):QObject(parent),name(name),weight(weight),height(height){

}
User::~User(){}
void User::addScan(Scan* scan){
    measurements.push_back(scan);
    qInfo() << "Scan:" << measurements.back() << measurements.last()->getDate().date();
   for(int num: measurements.last()->getPoints()){
       qInfo() << num;
   }
}

QString User::getName(){
    return name;
}

void User::setName(QString newName){
    name = newName;
    qInfo()<<"new name set";
}

void User::setWeight(int newWeight){
    weight = newWeight;
    qInfo()<<"new weight set";
}
void User::setHeight(int newHeight){
    height = newHeight;
    qInfo()<<"new height set";
}
//for graceful shutdown, removes the last scan
//this should be called in app if the timer is on
void User::deleteScan(){
    measurements.removeLast();
}
