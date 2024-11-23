#include "user.h"
#include "scan.h"
User::User(QObject* parent,QString name, int weight, int height):QObject(parent),name(name),weight(weight),height(height){

}
User::~User(){}
void User::addScan(Scan* scan){
    measurements.push_back(scan);
    qInfo() << 'Scan:' << measurements.back() << measurements.last()->getDate().date();
   for(int num: measurements.last()->getPoints()){
       qInfo() << num;
   }
}
