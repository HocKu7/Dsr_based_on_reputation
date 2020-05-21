#include "reputation.h"

using namespace std;
namespace ns3
{
    Reputation::Reputation(Ipv4Address addr)
    {
        m_mainIp = addr;
        delta = 0.1;
    }

    void Reputation::addToTable(Ipv4Address node, std::map<Ipv4Address, std::pair<int,int> > map){
        tableMapRep[node] = map;
    }

    void Reputation::incrementSendStat(Ipv4Address dst){
        tableMapRep[m_mainIp][dst].first++;
    }

    void Reputation::incrementRecvStat(Ipv4Address dst){
        tableMapRep[m_mainIp][dst].second++;
    }
    
    void
    Reputation::calculateMatrixS(){
        
        std::cout<<"calculated"<<std::endl;
        // for(std::map<Ipv4Address, std::map<Ipv4Address, std::pair<int, int> > > ::const_iterator nodeEntryMap = tableMapRep.begin();
        // nodeEntryMap!=tableMapRep.end(); nodeEntryMap++){

        //     Ipv4Address i_node = nodeEntryMap->first;
        //     std::map<Ipv4Address, std::pair<int, int> > nodeMap = nodeEntryMap->second;
            
        //     for(std::map<Ipv4Address, std::pair<int, int> > :: iterator it = nodeMap.begin(); it!=nodeMap.end();
        //     it++){

        //         Ipv4Address j_node = it->first;
        //     }
        // }
    }

    void 
    Reputation::calculateMatrixC(){

    }
    void
    Reputation::calculateReputationTable(){
        calculateMatrixS();
    }
} // namespace ns3