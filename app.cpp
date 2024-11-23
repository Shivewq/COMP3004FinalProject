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
