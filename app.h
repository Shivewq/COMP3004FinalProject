#ifndef APP_H
#define APP_H
#include <QVector>
#include <QDateTime>
#include <cmath>
#include <iostream>
#include <QDebug>
//foward declarations
class User;
class Device;
class App
{
public:
    App(Device* d);
    void calculateScan(int index); //gets the scan from the user and does calculations, not sure what it returns
    void measure();
    //I think below should be during the scan after getting the random number from the device it runs this function that returns an array of int. In the graph every say 0.3 seconds it plots a point.
    //in the graph, every 0.3 seconds pop a point. Stop the tiemr when there are no more points left
    QVector<int> calculateReadingGraph(int reading); //this function will take the reading and calculate the points on the graph based on the doc part "How we calculate reading/how we take measurement:"
    User* activeUser;

    //helper functions
    int randomNum(int minimum,int maximum);
private:
    Device* device;
    //User* activeUser;
    QVector<User*> users;
};

#endif // APP_H
