#ifndef Swarm_H
#define Swarm_H
#include <QGeoCoordinate>
#include <QDateTime>
#include <QList>
#include <QVector3D>
#include <algorithm>
#include <catalog.h>

/*!
 * \enum clusT
 * \brief The enum ClusT holds the cluster(swarm class) type which can be swarm, sequence, or unclasified or unknown.
 */
enum clusT
{
    //! Unclassified is the default category.
    unc,
    //! Unknown is given to a cluster that cannot be classified.
    unk,
    //! Swarm is the category for swarmic behaiviour.
    swm,
    //! Sequence is the category for seismic sequences.
    seq
};
Q_DECLARE_METATYPE(clusT);

/*!
 * \enum Trend
 * \brief The enum Trend holds the cluster(swarm class) trend type which can be NS Trend, EW Trend, NS-EW Trend with optional Deep, Shallow connotations
 */
enum trendT
{
    //! Unclassified is the default category
    ncl,
    //! None (no trend)
    none,
    //! North - South trend.
    ns,
    //! East - West trend.
    ew,
    //! Both NS and EW trend.
    nsew,
    //! Deep events
    deep,
    //! Shallow events
    shal,
    //! NS and shallow trend.
    nss,
    //! NS and deep trend.
    nsd,
    //! EW and shallow trend.
    ews,
    //! EW and deep trend.
    ewd,
    //! NS, EW and shallow trend.
    nsews,
    //! NS, EW and deep trend.
    nsewd
};
Q_DECLARE_METATYPE(trendT);

/*!
 * \class Swarm
 * \brief The Swarm class holds a Swarm object, which has an ID and a list of events.
 * \ingroup miscellaneous
 *
 * The Swarm class is a special case of Catalog where different events can be added to it,
 * provided it meets with a certain swarm ID. This is both to simplify code readability and
 * differing it from the main catalog.
 *
 */
class Cluster : public Catalog
{
public:
    Cluster();
    int  getID() const;
    bool setClusterID(int sid);
    bool operator==(const Cluster& check);
    bool operator==(const int& iid);
    bool setClusterTypeTrend(clusT type,trendT trend);
    QString getClass(bool shorthand);
    QString getTrend(bool shorthand);
    bool isclassified();
    clusT getClusterType();

private:
    int id;
    clusT mytype;
    trendT mytrend;
    bool classified;

};

#endif // Swarm_H
