#ifndef Catalog_H
#define Catalog_H
#include <QGeoCoordinate>
#include <QDebug>
#include <QDateTime>
#include <QList>
#include <QVector>
#include <event.h>
#include <cmath>
#include <algorithm>
#include <alglib/statistics.h>

/*!
 * \class Catalog
 * \brief The Catalog class holds a Catalog object, which has a list of events.
 * \ingroup miscellaneous
 *
 * The Catalog class has several functions that allow it to \ref Add_events and \ref Retrive_events.
 * It also contains a set of functions that allow it to \ref Event_stat Finally it also contains a way to
 * \ref subcat
 */
class Catalog
{
public:
    Catalog();
    /*!
     * \ingroup Add_events
     * \defgroup Add_events add events to catalog or swarm
     * @{
     */
    bool operator <<(eevent toAdd);
    bool addEvent(eevent toAdd);
    bool addEvent(QGeoCoordinate pos, QDateTime tim, double mag);
    /*!
     *@}
     */

    /*! \ingroup Retrive_events
     * \defgroup Retrive_events retrive a list of events in different formats.
     * @{
     */
    int  numEvents() const;
    eevent getEvent(int eid) const;
    QVector<double> getSeries(evinf type);
    alglib::real_1d_array getAlgSeries(evinf type);
    /*!
     *@}
     */

    /*! \ingroup Event_stat
     * \defgroup Event_stat get many basic statistics, such as min,max,avg,etc.
     * @{
     */
    QDateTime getMinTime();
    QDateTime getMaxTime();
    double getMax(evinf type);
    double getMin(evinf type);
    double getAvg(evinf type);
    double getMedian(evinf type);
    double getVariance(evinf type);
    double getStdDev(evinf type);
    bool getHistSeries(QVector<double> *key, QVector<double> *value, evinf type, double interval);
    /*!
     *@}
     */

    /*! \ingroup subcat
     * \defgroup subcat subcatalog parts of the catalog based on time-space criteria.
     * @{
     */
    Catalog getSubCat(QGeoCoordinate TopLeft, QGeoCoordinate BottomRight, QDateTime Start, QDateTime End);
    /*!
     *@}
     */
private:
    QMap<QDateTime, eevent> eventsmap;
};

#endif // Catalog_H
