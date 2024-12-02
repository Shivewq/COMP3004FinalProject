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
    ~App();

    //data collection and processing
    QVector<int> calculateScan(QVector<int> rawPoints);
    void MeasureFunctionTemplate();
    QVector<int> calculateReadingGraph(int reading);

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
    User* activeUser;
    QVector<User*> users;
    bool scanning;


signals:
    void plotPoint(int y);
    void clearMeteringGraph(int max_y,int max_x);
    void deleteCurrentScan();
    void bodyPointNumber(int *const);
    void bodyImageNum(int *const);
    void skinContact(bool isContact); //to tell the UI if the device is making contact with skin
    void outOfBattery(QString); // For when the battery is out
    void doneScan(); // Successful completion of scan

public slots:
    void stopMeasure();
};
#endif
