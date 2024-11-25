#include "app.h"
#include "device.h"
#include "user.h"
#include "scan.h"
#include <cmath>
#include <iostream>
#include <QDebug>
App::App(Device* d):device(d)
{

}
void App::measure(){
    //for all 24 points on the body
    QVector<int> measurement;
    for(int i = 0; i < 23; i++ ){
        measurement.push_back(this->device->geneateDataPoint());
    }
    QDateTime scanDate = QDateTime::currentDateTime();
    activeUser->addScan(new Scan(measurement,scanDate));
}
//45-70 is normal. < 45 is low functionality, > 70 is high functionality
void App::calculateScan(int index){

    QVector<int> processedScan;
    Scan* scan = activeUser->getScan(index);
    QVector<int> rawData = scan->getPoints();
    double processed; //the % difference

    //goes through each measurement and calculates the %.
    for(int point: rawData){
        if(point > 45 && point < 70){ //if the functionality is normal will be under 100%
            processed = 100;
        }
        else if(point < 45){ //if the functionality is low
            processed = std::abs(45-point);
            processed = 100 -((processed/45)*100);
        }
        else if(point > 70){ //if the functionality is high. will be over 100%
            processed = std::abs(point - 70);
            processed = 100 + ((processed/point)*100);
        }
        processedScan.push_back(std::floor(processed));
    }
}

//set the active user to the user selected in the profiles page
void App::setActiveUser(User* user){
    qInfo()<<"active user updated";
    activeUser = user;
}

//gets the active user
User* App::getActiveUser(){
    return activeUser;

}

//add user to users list
void App::addUser(User* user){
    qInfo()<<"User added to list";
    users.append(user);

}

//deletes user from user list
void App::deleteUser(User* user){
    qInfo()<<"User removed";
    int index = users.indexOf(user);
    if (index != -1) {
        users.removeAt(index);
    }

    delete user;
}

//updates user profile data
void App::updateUser(User* user){

}

//gets the user object from their name
User* App:: getUserFromName(QString name){
    for(User* user: users){
        if (user->getName() == name){
            return user;
        }
    }
    return nullptr;
}


