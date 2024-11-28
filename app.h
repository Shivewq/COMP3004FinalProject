#ifndef APP_H
#define APP_H
#include <QObject>
#include <QVector>
#include <QDateTime>
#include <cmath>
#include <iostream>
#include <QDebug>
#include <QTimer>
#include <QString>
//foward declarations
class User;
class Device;
class App :public QObject
{
    Q_OBJECT
public:
    App(Device* d);
    QVector<int> calculateScan(QVector<int> rawPoints); //gets the scan from the user and does calculations, not sure what it returns
    void MeasureFunctionTemplate();
    //I think below should be during the scan after getting the random number from the device it runs this function that returns an array of int. In the graph every say 0.3 seconds it plots a point.
    //in the graph, every 0.3 seconds pop a point. Stop the tiemr when there are no more points left
    QVector<int> calculateReadingGraph(int reading); //this function will take the reading and calculate the points on the graph based on the doc part "How we calculate reading/how we take measurement:"
    User* activeUser;
    //helper functions
    int randomNum(int minimum,int maximum);

    //getters and setters
    bool isScanning(){return scanning;}
    void setActiveUser(User*);
    User* getActiveUser();
    void addUser(User*);
    void deleteUser(User*);
    User* getUserFromName(QString name);
    QStringList categories = {
        "H1", "H2", "H3", "H4", "H5", "H6", // Hand points
        "F1", "F2", "F3", "F4", "F5", "F6"  // Foot points
    };


private:
    Device* device;
    //User* activeUser;
    QVector<User*> users;
    bool scanning;
//private slots:
   // void takeMeasurement(int counter, QVector<int*> measurements); //takes in the current point we are measureing and the vector of all previous points.
signals:
    void plotPoint(int y);
    void clearMeteringGraph(int max_y,int max_x); //possibly pass in final value for the axis in case the graph doesn't dynamically change it in the case that the final value is very large.
    void deleteCurrentScan();
    void bodyPointNumber(int *const); // For UI changes
    void bodyImageNum(int *const); //Also for UI
    void skinContact(bool isContact); //to tell the UI if the device is making contact with skin
public slots:
    void stopMeasure();
};
#endif
