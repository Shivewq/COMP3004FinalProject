#ifndef SCAN_H
#define SCAN_H
#include <QVector>
#include <QObject>
#include <QString>
#include <QDateTime>
//45-70 is normal. < 45 is low functionality, > 70 is high functionality
class Scan : public QObject
{
    Q_OBJECT
public:
    explicit Scan(QVector<int> scan, QVector<int> processed,QDateTime date, QObject* parent = nullptr);
    QVector<int> getPoints(){return points;}
    QVector<int> getProccesedPoints(){return processedPoints;}
    QDateTime getDate(){return date;}
    QString getName(){return name;}
    QString toString() const;
private:
    QVector<int> points; //stores all 24 readings from device
    QVector<int> processedPoints;
    QDateTime  date; //date of scan
    QString name; //name of scan
};

#endif // SCAN_H
