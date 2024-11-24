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
     User* activeUser;
private:
    Device* device;
    //User* activeUser;
    QVector<User*> users;
};

#endif // APP_H
