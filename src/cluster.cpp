#include "cluster.h"

/*!
 * \brief Swarm::Swarm Initializes the id to zero and clears the event list.
 *
 */
Cluster::Cluster():Catalog()
{
    id = 0;
    mytype = unk;
    mytrend = ncl;
    classified = false;
}

/*!
 * \brief Swarm::operator == Compares a Swarm against another Swarm. Only does an ID comparison.
 * \param check The Swarm to be compared.
 * \return True if they are equal, false otherwise.
 */
bool Cluster::operator==(const Cluster& check){
    if(id == check.getID())
        return true;
    return false;
}

/*!
 * \brief Swarm::operator == Compares a swarm against an ID.
 * \param iid An interger ID.
 * \return True if it's has the id IID, false otherwise.
 */
bool Cluster::operator ==(const int& iid){
    if (id == iid)
        return true;
    return false;
}

/*!
 * \brief Swarm::setSwarmTypeTrend is used to set both the type and the trend of the swarm(cluster).
 * \param type The type of the cluster (swm, seq, unk).
 * \param trend The trend of the cluster(NS,EW,None, etc);
 * \return True if it has been set correctly, false otherwise.
 */
bool Cluster::setClusterTypeTrend(clusT type, trendT trend)
{
    mytrend = trend;
    mytype = type;
    classified = true;
    return classified;
}

QString Cluster::getClass(bool shorthand)
{
    if(classified){
        switch (mytype) {
        case unk:
            if (shorthand)
                return "UNK";
            else
                return "Unknown";
            break;
        case swm:
            if (shorthand)
                return "SWM";
            else
                return "Swarm";
            break;
        case seq:
            if (shorthand)
                return "SEQ";
            else
                return "Sequence";
            break;
        default:
            return "What?!";
            break;
        }
    }
    else{
        if (shorthand)
            return "UNC";
        else
            return "Unclassified";
    }
}

QString Cluster::getTrend(bool shorthand)
{
    if(classified){
        switch (mytrend) {
        case none:
            if(shorthand)
                return "NT";
            else
                return "No_Trend";
            break;
        case ns:
            if(shorthand)
                return "NS";
            else
                return "NS_Trend";
            break;
        case ew:
            if(shorthand)
                return "EW";
            else
                return "EW_Trend";
            break;
        case nsew:
            if(shorthand)
                return "NS_EW";
            else
                return "NS_EW_Trend";
            break;
        case deep:
            if(shorthand)
                return "DP";
            else
                return "Deep_events";
            break;
        case shal:
            if(shorthand)
                return "SH";
            else
                return "Shallow events";
            break;
        default:
                return "What?!";
            break;
        }
    }
    else{
        if(shorthand)
            return "UNC";
        else
            return "Unclassified";
    }
}

bool Cluster::isclassified()
{
    return classified;
}

clusT Cluster::getClusterType()
{
    if (classified)
        return mytype;
    return unc;
}

/*!
 * \brief Swarm::getID Returns the ID of the Swarm.
 * \return Swarm ID as an int.
 */
int Cluster::getID() const{
    return id;
}

/*!
 * \brief Swarm::setSwarmID is used to set the ID of the swarm initially. Can only be used once.
 * \param sid integer ID to set the swarm ID.
 * \return True if succesfully set, false if otherwise.
 */
bool Cluster::setClusterID(int sid){
    if(id == 0){
        id = sid;
        return true;
    }
    return false;
}
