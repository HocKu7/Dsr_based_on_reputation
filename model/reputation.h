#ifndef EIGEN_H
#define EIGEN_H

#include <string>
#include <map>
#include "ns3/ipv4.h"
#include "utills.h"
#include "/home/vanet/tarballs/EigenLib/eigen-3.2.10/Eigen/Dense"
#include <algorithm> 

using namespace std;
namespace ns3
{
    class Reputation
    {

    private:
        Ipv4Address m_mainIp;
        Eigen::MatrixXd matrixC;
        Eigen::MatrixXd matrixS;
        std::map<Ipv4Address, std::map<Ipv4Address, std::pair<int, int> > > tableMapRep;
        std::map<Ipv4Address, int> ipToLineMap;
        double delta;

        void calculateMatrixC();
        void calculateMatrixS();
        int getSij(Ipv4Address i, Ipv4Address j);
        bool hasInList(std::list<Ipv4Address> list,  Ipv4Address ip);
        void printMap();

    public:
        Reputation(Ipv4Address addr);
        void addToTable(Ipv4Address node, std::map<Ipv4Address, std::pair<int,int> > map);
        void incrementSendStat(Ipv4Address dst);
        void incrementRecvStat(Ipv4Address dst);
        void calculateReputationTable();
    };
} // namespace ns3
#endif /* EIGEN_H */