#ifndef SCAN_H
#define SCAN_H
#include <QVector>
#include <QString>
#include <QDateTime>
class Scan
{
public:
    Scan(QVector<int> scan, QDateTime date);
    QVector<int> getPoints(){return points;}
    QDateTime getDate(){return date;}
    QString getName(){return name;}
private:
    QVector<int> points; //stores all 24 readings from device
    QDateTime  date; //date of scan
    QString name; //name of scan
};

#endif // SCAN_H
