#ifndef SCAN_H
#define SCAN_H
#include <QVector>
#include <QString>
class Scan
{
public:
    Scan();
private:
    QVector<int> points; //stores all 24 readings from device
    QString date; //date of scan
    QString name; //name of scan
};

#endif // SCAN_H
