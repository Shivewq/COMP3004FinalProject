#include "app.h"
#include "device.h"
#include "user.h"
#include "scan.h"
#include <QDebug>
App::App(Device* d):device(d)
{

}

App::~App(){
    //delete all the users stored in the users vector
    for(User* u: users){
        delete u;
    }

}

//store all the raw data points generated from the device in the measurments vector
//plot all the datapoints collected on the graph in the measure page
void App::MeasureFunctionTemplate(){
    if(!device->isOn()) return;
    if(device->getBattery()->getCharge() <= 0){
        device->turnOff();
        emit outOfBattery(QString(""));
        return;
    }
    scanning = true;
    //pre-get all data points
    QVector<int> measurement;
    for(int i = 0; i < 24; i++ ){
        measurement.push_back(this->device->geneateDataPoint());
    }
    QDateTime scanDate = QDateTime::currentDateTime();
    //store the raw data points and processed data in a scan object
    activeUser->addScan(new Scan(measurement,calculateScan(measurement),scanDate));

    //timer is to loop through each point. At the end of the function restart the timer
    int* counter = new int(0); //increments when me move through the points.
    QTimer* points = new QTimer(this);
    points->setSingleShot(true);
    connect(points,&QTimer::timeout,this,[this,measurement,counter,points](){
        //If device runs out during scan display out of battery
        if(device->getBattery()->getCharge() <= 0){
            stopMeasure();
            emit outOfBattery(QString("Current Scan Stopped!"));
            scanning = false;
            return;
        }
        if(*counter == 24){
            scanning = false;
            emit doneScan();
            return;
        }
        int data = measurement.at(*counter);
        //display the skin contact, scan point number and the correct image
        emit skinContact(true);
        emit bodyPointNumber(counter);
        emit bodyImageNum(counter);

        //plot the points
        QVector<int>* graph_Yvalues = new QVector<int>(calculateReadingGraph(data));
        QTimer* graph = new QTimer(); //will declare with "this" later as a parameter to set the parent object
        graph->setSingleShot(true);
        emit clearMeteringGraph(data,graph_Yvalues->size());
        connect(graph,&QTimer::timeout, this,[this,graph_Yvalues,graph,counter,points](){

            if(!graph_Yvalues->isEmpty()){ //if there is still more to plot
                int y = graph_Yvalues->takeFirst();
                //plot the point by popping a value from the front of y
                emit plotPoint(y);
                if(scanning) //this will only be false if a graceful shutdown has happened
                    graph->start(50); //restart the timer
            }
            else{ //if there is no more points to graph. We move onto the next measurement
                qInfo() << "counter" <<*counter;
                *counter+= 1;
                if(scanning) //this will only be false if a graceful shutdown has happened
                    emit skinContact(false);
                    points->start(100);
            }

        });
       graph->start(400);
    });
    points->start(400);
}


//gets the raw data from the device, and processes them to get the functionality percentages of that organ
//returns a vector of all the processed data
QVector<int> App::calculateScan(QVector<int> rawPoints){
    double processed; //the % difference
    QVector<int> processedPoints;
    //goes through each measurement and calculates the %.
    //45-70 is normal. < 45 is low functionality, > 70 is high functionality
    for(int point: rawPoints){
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
        qInfo() << "Data point: " <<point << "Processed value:" << processed;
        processedPoints.push_back(std::floor(processed));
    }
    return processedPoints;
}



//takes in an individual reading point, and outputs a vector of all the Y axis points for the movement of the reading graph on the measure page
QVector<int> App::calculateReadingGraph(int reading){
    QVector<int> yValues;
    int y = 0; //current i

    //basically for 1/4 of reading, 1/2 reading, 3/4 reading , full reading
    for(int i = 1; i <= 4; i++){
        y = (i*reading)/4;
        //i = 1, 1/4 reading, i = 2, 1/2 reading, i = 3, 3/4 reading, i = 4, full reading
        int f = randomNum(1,2); // how much difference to add to simulate random change between i intervals.

        //it will still simulate a longer reading
        for(int i = 0; i < randomNum(1,3); i++){
            yValues.push_back(y - f);
        }
        for(int i = 0; i < randomNum(1,3);i++){
             yValues.push_back(y);
        }
    }
    return yValues;
}



//helper functions

//generates a random number between minimum and maximum
int App::randomNum(int minimum,int maximum){
    std::random_device rd; // Non-deterministic random seed
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<> dist(minimum,maximum); // Distribution in range [min, max]. //before change
    return dist(gen);
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


//gets the user object from their name
User* App:: getUserFromName(QString name){
    for(User* user: users){
        if (user->getName() == name){
            return user;
        }
    }
    return nullptr;
}

//when the app runs out of battery, this stops the timers so no more points are plotted on the graph
void App::stopMeasure(){
    //if we are currently measuring need to delete the most recent scan from the user list
    if(scanning) {
        emit deleteCurrentScan();
        scanning = false;
    }

}
