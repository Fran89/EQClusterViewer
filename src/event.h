#ifndef EVENT_H
#define EVENT_H
#include <QGeoCoordinate>
#include <QDateTime>
#include <QList>
/*!
 * \file event.h
 */


/*!
 * \brief The eevent struct is event structure, named eevent because of it would clash with the event function of a QObject class,
 * contains the main information about an event and if within a catalog can be called with the get(type) functions if you know the
 * \ref evinf enum.
 */
struct eevent{
    //! Position of the event in a QGeoCoordinate and Altitude(Depth in Meters).
    QGeoCoordinate position;
    //! Time of the event in a QDateTime.
    QDateTime time;
    //! Time of the event in number of miliseconds since the start of the Unix epoch: midnight UTC of January 1, 1970 (not counting leap seconds)
    time_t time_in_usec;
    //! Decimal Latitude
    double latitude;
    //! Decimal Longitude
    double longitude;
    //! Depth in Km
    double depth;
    //! Magnitude
    double magnitude;
};

/*!
 * \enum evinf
 *  This enum type specifies a the type of event information to return in a double type.
 */
enum evinf{
    //! Latitude of event(s) returned in double
    Lat,
    //! Longitude of event(s) returned in double
    Lon,
    //! Time_t (Msec from epoch) of event(s) returned in double
    Time,
    //! Time_t (sec from epoch) of event(s) returned in double
    TimeS,
    //! Depth of event(s) in Km returned in double
    Depth,
    //! Magnitude of event(s) returned in double
    Mag
};

#endif // EVENT_H
