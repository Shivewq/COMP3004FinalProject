#ifndef USER_H
#define USER_H
#include <QString>
#include <QVector>
#include <QObject>
#include <QDebug>
class Scan; //foward decalration of scan for the scan list
class User: public QObject{
        Q_OBJECT

public:
    explicit User(QObject* parent = nullptr, QString name = "name", int weight = 53, int height = 180);
    ~User();
    void addScan(Scan* scan);
    Scan* getScan(int index){return measurements.at(index);}

    QString getName();
    int getHeight();
    int getWeight();
    void setName(QString name);
    void setWeight(int weight);
    void setHeight(int height);

private:
    QString name;
    int weight; //in kg
    int height; // in cm
    QVector<Scan*> measurements; //back is most recent scan
};

#endif // USER_H
