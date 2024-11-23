#ifndef APP_H
#define APP_H
#include <QVector>
class User;
class App
{
public:
    App();
    void calculateScan(int index); //gets the scan from the user and does calculations, not sure what it returns
private:
    User* activeUser;
    QVector<User*> users;
};

#endif // APP_H
