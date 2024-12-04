#include "user.h"
#include "scan.h"
User::User(QObject* parent,QString name, int weight, int height):QObject(parent),name(name),weight(weight),height(height){

}
User::~User(){}

//adds scab to the measurements vector
void User::addScan(Scan* scan){
    measurements.push_back(scan);
    qInfo() << "Scan:" << measurements.back() << measurements.last()->getDate().date();
   for(int num: measurements.last()->getPoints()){
       qInfo() << num;
   }
}

//returns the users name, height, weight
QString User::getName(){
    return name;
}

int User::getHeight(){
    return height;
}

int User::getWeight(){
    return weight;
}

//sets the users name
void User::setName(QString newName){
    name = newName;
    qInfo()<<"new name set";
}

//sets the users weight
void User::setWeight(int newWeight){
    weight = newWeight;
    qInfo()<<"new weight set";
}

//sets the users height
void User::setHeight(int newHeight){
    height = newHeight;
    qInfo()<<"new height set";
}

//for graceful shutdown, removes the last scan
void User::deleteScan(QString name){
    if(name != this->name) return; //if this is not the right user to delete
    measurements.removeLast();
}
