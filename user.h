#ifndef USER_H
#define USER_H
#include <QString>
#include <QVector>
#include <QObject>

class Scan; //foward decalration of scan for the scan list
class User: public QObject{
        Q_OBJECT

public:
    explicit User(QObject* parent = nullptr);
    ~User();
private:
    QString name;
    int weight; //in kg
    int height; // in cm
    QVector<Scan*> measurements;
};

#endif // USER_H
