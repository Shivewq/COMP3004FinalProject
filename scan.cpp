#include "scan.h"

Scan::Scan(QVector<int> scan,QVector<int> processed, QDateTime date, QObject* parent):QObject(parent), points(scan),processedPoints(processed),  date(date)
{

}


//gives each scan a title and shows the date and time it was taken
QString Scan::toString() const {
    return QString("Scan taken on %1").arg(date.toString("yyyy-MM-dd HH:mm:ss"));
}
