#include "app.h"
#include "device.h"
#include "user.h"
#include "scan.h"
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
        qInfo() <<"Data point: " <<point << "Processed value:" << processed;
        processedScan.push_back(std::floor(processed));
    }
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
    qInfo() <<"Calculating graph points based on reading:" << reading;
    for(int num: yValues){
        qInfo() << num;
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
