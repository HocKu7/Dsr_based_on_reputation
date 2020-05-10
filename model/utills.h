#ifndef UTILS_H
#define UTILS_H

#include "ns3/ipv4-address.h"
#include <iostream>
#include <iterator>
#include <list>

using namespace std;
namespace ns3 {
class Utill{

private:

public:

    static Ipv4Address findFirst(std::list<Ipv4Address> ipList, Ipv4Address ip);

    static bool deleteFirst(std::list<Ipv4Address> ipList, Ipv4Address ip);

};
}
#endif /* UTILS_H */