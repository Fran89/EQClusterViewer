#include "catalog.h"

/*!
 * \brief Catalog::Catalog Initializes the id to zero and clears the event list.
 *
 */
Catalog::Catalog()
{
    eventsmap.clear();
}

/*!
 * \brief Catalog::operator << Overloads the << operator to be able to add events.
 * \param toAdd Event to be added.
 * \return True if successfully added, else returns false.
 */
bool Catalog::operator << (eevent toAdd){
    return addEvent(toAdd);
}

/*!
 * \brief Catalog::numEvents Returns the total number of events in the Catalog.
 * \return Total events in Catalog.
 */
int Catalog::numEvents() const{
    return eventsmap.size();
}

/*!
 * \brief Catalog::getEvent Returns an event from the Catalog provided it exists. EID sent must be between 0 and numEvents().
 * Else it returns an empty event with the time_in_usec set to -1.
 * \param eid ID of the requested event.
 * \return Event with EID, if exist. Else it returns an empty event with time_in_usec set to -1.
 */
eevent Catalog::getEvent(int eid) const{
    if(eid >= 0 && eid < numEvents())
        return eventsmap.values().at(eid);
    eevent temp;
    temp.time_in_usec = -1;
    return temp;
}

/*!
 * \brief Catalog::addEvent Adds an Event to the Catalog
 * \param toAdd Event to append to the Catalog
 * \return True if succesfully added, false otherwise.
 */
bool Catalog::addEvent(eevent toAdd){
    eventsmap.insert(toAdd.time,toAdd);
    return true;
}

/*!
 * \brief Catalog::addEvent Used to add an event when you have Position, Time and Magnitude.
 * \param pos Position in form QGeoCoordinate.
 * \param tim Time in QDateTime.
 * \param mag Magnitude in double.
 * \return true if successfully added, false otherwise.
 */
bool Catalog::addEvent(QGeoCoordinate pos, QDateTime tim, double mag){
    eevent temp;
    temp.time = tim;
    temp.position = pos;
    temp.magnitude = mag;
    temp.latitude = pos.latitude();
    temp.longitude = pos.longitude();
    temp.depth = pos.altitude()/1000;
    temp.time_in_usec = tim.toTime_t();
    return addEvent(temp);
}

/*!
 * \brief Catalog::getSeries funcion retrives a vector of the requested type (\ref evinf)
 * \param type Type of data requested
 * \return A vector of the requested event data type as double (time must be converted back from Msec since epoch)
 */
QVector<double> Catalog::getSeries(evinf type){
    QVector<double> temp;
    if (numEvents() > 0){
        for (int i=0; i < numEvents(); i++){
            switch (type) {
            case Lat:
                temp.append(getEvent(i).latitude);
                break;
            case Lon:
                temp.append(getEvent(i).longitude);
                break;
            case Time:
                temp.append(getEvent(i).time.toMSecsSinceEpoch());
                break;
            case TimeS:
                temp.append(getEvent(i).time.toTime_t());
                break;
            case Depth:
                temp.append(getEvent(i).depth);
                break;
            case Mag:
                temp.append(getEvent(i).magnitude);
                break;
            default:
                qDebug() << "You shouldn't be here\n";
                break;
            }
        }
    }
    return temp;
}

/*!
 * \brief Catalog::getSeries funcion retrives a vector of the requested type (\ref evinf)
 * \param type Type of data requested
 * \return A vector of the requested event data type as double (time must be converted back from Msec since epoch)
 */
alglib::real_1d_array Catalog::getAlgSeries(evinf type){
    alglib::real_1d_array temp;
    if(numEvents() > 0){
    temp.setlength(numEvents());
        for (int i=0; i < numEvents(); i++){
            switch (type) {
            case Lat:
                temp(i) = getEvent(i).latitude;
                break;
            case Lon:
                temp(i) =getEvent(i).longitude;
                break;
            case Time:
                temp(i) = getEvent(i).time.toMSecsSinceEpoch();
                break;
            case TimeS:
                temp(i) = getEvent(i).time.toTime_t();
                break;
            case Depth:
                temp(i) = getEvent(i).depth;
                break;
            case Mag:
                temp(i) = getEvent(i).magnitude;
                break;
            default:
                qDebug() << "You shouldn't be here\n";
                break;
            }
        }
    }
    return temp;
}

/*!
 * \brief Catalog::getMaxTime
 *  Returns the most recent time of the Catalog or Swarm
 * \return Most recent time of Catalog.
 */
QDateTime Catalog::getMaxTime(){
    QDateTime temp = QDateTime::fromMSecsSinceEpoch(getMax(Time));
    return temp;
}

/*!
 * \brief Catalog::getMinTime
 * Returns the most first event time of the Catalog or Swarm
 * \return First event time of Catalog
 */
QDateTime Catalog::getMinTime(){
    QDateTime temp = QDateTime::fromMSecsSinceEpoch(getMin(Time));
    return temp;
}

/*!
 * \brief Catalog::getMax
 * Returns the upper bound of selected type (\ref evinf) for the Catalog or Swarm
 * \param type Type of data requested
 * \return Upper bound of selected type;
 */
double Catalog::getMax(evinf type){
    if(numEvents() > 0){
        QVector<double> temp = getSeries(type);
        return *std::max_element(temp.constBegin(),temp.constEnd());
    }
    return 0;
}

/*!
 * \brief Catalog::getMin
 * Returns the lower bound of selected type (\ref evinf) for the Catalog or Swarm
 * \param type Type of data requested
 * \return Lower bound of selected type
 */
double Catalog::getMin(evinf type){
    if(numEvents() > 0){
        QVector<double> temp = getSeries(type);
        return *std::min_element(temp.constBegin(),temp.constEnd());
    }
    return 0;
}

/*!
 * \brief Catalog::getAvg
 * Returns the average of selected type (\ref evinf) for the Catalog or Swarm
 * \param type Type of data requested
 * \return Average of selected type
 */
double Catalog::getAvg(evinf type){
    if(numEvents() > 0){
        return alglib::samplemean(getAlgSeries(type));
    }
    return 0;
}

/*!
 * \brief Catalog::getMedian
 * Returns the average of selected type (\ref evinf) for the Catalog or Swarm
 * \param type Type of data requested
 * \return Median of selected type
 */
double Catalog::getMedian(evinf type){
    if(numEvents() > 0){
        double temp = 0;
        alglib::samplemedian(getAlgSeries(type),temp);
        return temp;
    }
    return 0;
}

/*!
 * \brief Catalog::getVariance
 * Returns the variance of selected type (\ref evinf) for the Catalog or Swarm
 * \param type Type of data requested
 * \return Variance of selected type
 */
double Catalog::getVariance(evinf type){
    if(numEvents() > 0){
        return alglib::samplevariance(getAlgSeries(type));
    }
    return 0;
}

/*!
 * \brief Catalog::getStdDev
 * Returns the standard deviation of selected type (\ref evinf) for the Catalog or Swarm
 * \param type Type of data requested
 * \return Standard Deviation of selected type
 */
double Catalog::getStdDev(evinf type){
    if(numEvents() > 0){
        return sqrt(getVariance(type));
    }
    return 0;
}

/*!
 * \brief Catalog::getHistSeries
 * Returns the Histograms of selected type (\ref evinf) for the Catalog or Swarm
 * \param key Vector with keys (middle of bars)
 * \param value Vector with the values (height of bars)
 * \param type Type of data requested (can only be mag or depth)
 * \return True if successful, false otherwise.
 */
bool Catalog::getHistSeries(QVector<double> *key, QVector<double> *value, evinf type, double interval)
{
    int min, max;
    QVector<double> rounded;
    if (!((type == Depth) || (type == Mag)))
        return false;

    if( type == Depth) {
        for(int i = 0; i < numEvents(); i++){
            // round to 0.1
            rounded.append(std::round(eventsmap.values().at(i).depth*10.0) / 10.0);
        }
    }
    if( type == Mag) {
        for(int i = 0; i < numEvents(); i++){
            // round to 0.1
            rounded.append(std::round(eventsmap.values().at(i).magnitude*10.0) / 10.0);
        }
    }
    if (rounded.size() != numEvents())
        return false;

    min = std::floor(getMin(type));
    max = std::ceil(getMax(type));
    QVector<double>::ConstIterator it;

    for(double i = min; i <= max; i = i+interval){
        int count = 0;
        it = rounded.constBegin();
        while (it != rounded.constEnd()){
            if( i <= *it && *it < (i+interval))
                count++;
            it++;
        }
        if(count > 0){
            value->append(count);
            key->append((i+(i+interval))/2);
        }
    }
    if (value->size() == key->size())
        return true;
    return false;
}

/*!
 * \brief Catalog::getSubCat Returns a catalog with all the found events for a space-time search.
 * \param TopLeft Top Left (Min Long, Max Lat in the West Hem) of the seach area.
 * \param BottomRight Bottom Right (Max Long, Min Lat in the West Hem) of the search area.
 * \param Start Start time of the search in QDateTime
 * \param End End time of the search in QDateTime
 * \return A SubCataog with all the events found for that area at that time
 */
Catalog Catalog::getSubCat(QGeoCoordinate TopLeft, QGeoCoordinate BottomRight, QDateTime Start, QDateTime End){
    Catalog temp;
    QMap<QDateTime, eevent>::const_iterator i = eventsmap.begin();
    while(i != eventsmap.end()){
        if(Start <= i.key() && i.key() <= End){
            if(BottomRight.latitude() <= i.value().latitude && i.value().latitude <= TopLeft.latitude() &&
                    TopLeft.longitude() <= i.value().longitude && i.value().longitude <= BottomRight.longitude()){
                temp << i.value();
            }
        }
        i++;
    }
    return temp;
}
