#include "app.h"
#include "device.h"
#include "user.h"
#include "scan.h"
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
