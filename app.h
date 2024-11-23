#ifndef APP_H
#define APP_H
#include <QObject>
#include <QVector>
#include <QDateTime>
#include <cmath>
#include <iostream>
#include <QDebug>
//foward declarations
class User;
class Device;
class App :public QObject
{
    Q_OBJECT
public:
    App(Device* d);
    int calculateScan(int index); //gets the scan from the user and does calculations, not sure what it returns
    //void measure();
    void MeasureFunctionTemplate();
    //I think below should be during the scan after getting the random number from the device it runs this function that returns an array of int. In the graph every say 0.3 seconds it plots a point.
    //in the graph, every 0.3 seconds pop a point. Stop the tiemr when there are no more points left
    QVector<int> calculateReadingGraph(int reading); //this function will take the reading and calculate the points on the graph based on the doc part "How we calculate reading/how we take measurement:"
    User* activeUser;
    //void graphFunction(QVector<int>* yValues); //will remove
    //helper functions
    int randomNum(int minimum,int maximum);
private:
    Device* device;
    //User* activeUser;
    QVector<User*> users;
//private slots:
   // void takeMeasurement(int counter, QVector<int*> measurements); //takes in the current point we are measureing and the vector of all previous points.
signals:
    void plotPoint(int y);
    void clearMeteringGraph(); //possibly pass in final value for the axis in case the graph doesn't dynamically change it in the case that the final value is very large.
};

#endif // APP_H
