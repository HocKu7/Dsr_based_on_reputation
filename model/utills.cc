#include "utills.h"

namespace ns3
{
    bool
    Utill::deleteFirst(std::list<Ipv4Address> ipList, Ipv4Address ip)
    {
        for (std::list<Ipv4Address>::iterator it = ipList.begin(); it != ipList.end(); it++)
        {
            if (it->IsEqual(ip))
            {
                ipList.erase(it);
                return true;
            }
        }
        return false;
    }

    Ipv4Address
    Utill::findFirst(std::list<Ipv4Address> ipList, Ipv4Address ip)
    {
        for (std::list<Ipv4Address>::iterator it = ipList.begin(); it != ipList.end(); it++)
        {
            if (it->IsEqual(ip))
            {
                return *it;
            }
        }

        return NULL;
    }
} // namespace ns3