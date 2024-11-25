#include "app.h"
#include "device.h"
#include "user.h"
#include "scan.h"
#include <QDebug>
App::App(Device* d):device(d),graphTimer(new QTimer(this)),points(new QTimer(this))
{
    points->setSingleShot(true);
    graphTimer->setSingleShot(true);
}
//will have signal to plot point and clear graph
//function template for starting measuring, getting the reading,calculating the graph points
//will need a slot in mainwindow to connect to app that updates the thing that displays which point we are measuring
void App::MeasureFunctionTemplate(){
    //if the device is off or it has no charge left
    qInfo() <<"BATTERY:" <<device->getBattery()->getCharge() <<"IS ON" <<device->isOn();
    if(!device->isOn() || device->getBattery()->getCharge() == 0) return;

    //pre-get all data points
    QVector<int> measurement;
    for(int i = 0; i < 23; i++ ){
        measurement.push_back(this->device->geneateDataPoint());
    }
    QVector<int> processedMeasurements = calculateScan(measurement);
    QDateTime scanDate = QDateTime::currentDateTime();
    qInfo() <<processedMeasurements;
    activeUser->addScan(new Scan(measurement,processedMeasurements,scanDate));
    //So basically the timer is to loop through each point. At the end of the function you restart the timer
    int* counter = new int(0); //increments when me move through the points. Pointer so I can modify in the timeout

    connect(points,&QTimer::timeout,this,[this,measurement,counter](){
        if(*counter == 23) {
            return;
        }
        int data = measurement.at(*counter);
        QVector<int>* graph_Yvalues = new QVector<int>(calculateReadingGraph(data));
        emit clearMeteringGraph(data,graph_Yvalues->size());

        connect(graphTimer,&QTimer::timeout, this,[this,graph_Yvalues,counter](){
            //emit clearMeteringGraph(graph_Yvalues->last());
            if(!graph_Yvalues->isEmpty()){ //if there is still more to plot
                int y = graph_Yvalues->takeFirst();
                //plot the point by popping a value from the front of y
                emit plotPoint(y);
                graphTimer->start(400); //restart the timer
            }
            else{ //if there is no more points to graph. We move onto the next measurement
                *counter+= 1;
                points->start(400);
            }

        });
       graphTimer->start(400);
    });
    points->start(400);
}

//45-70 is normal. < 45 is low functionality, > 70 is high functionality
QVector<int> App::calculateScan(QVector<int> rawPoints){
    double processed; //the % difference
    QVector<int> processedPoints;
    //goes through each measurement and calculates the %.
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
        qInfo() <<"Data point: " <<point << "Processed value:" << processed;
        processedPoints.push_back(std::floor(processed));
    }
    return processedPoints;
}
//input: an individual reading point
//output: A vector of all the y axis points for the graph
QVector<int> App::calculateReadingGraph(int reading){
    QVector<int> yValues;
    int y = 0; //current i thing
    //basically for 1/4 of reading, 1/2 reading, 3/4 reading , full reading
    for(int i = 1; i <= 4; i++){
        y = (i*reading)/4;
        //i = 1, 1/4 reading, i = 2, 1/2 reading, i = 3, 3/4 reading, i = 4, full reading
        int f = randomNum(1,2); // how much difference to add to simulate random change between i intervals. Could remove if graph looks weird with low values of reading such as 8.
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
    if(graphTimer->isActive() || points->isActive()) emit deleteCurrentScan();
    graphTimer->stop();
    points->stop();

}
