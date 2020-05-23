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
        std::list <Ipv4Address> ipList;

        for(std::map<Ipv4Address, std::map<Ipv4Address, std::pair<int, int> > > ::const_iterator nodeEntryMap = tableMapRep.begin();
        nodeEntryMap!=tableMapRep.end(); nodeEntryMap++){

            std::map<Ipv4Address, std::pair<int, int> > nodeMap = nodeEntryMap->second;
            
            for(std::map<Ipv4Address, std::pair<int, int> > :: iterator it = nodeMap.begin(); it!=nodeMap.end();
            it++){

                Ipv4Address j_node = it->first;
                
                if(!hasInList(ipList, j_node)){
                    ipList.push_back(j_node);
                }
            }
        }

        matrixS.resize(ipList.size(), ipList.size());
        int curentRow=0;
        for(std::list<Ipv4Address> :: iterator i = ipList.begin(); i!=ipList.end(); i++){
            Eigen::VectorXd vectorStat;
            vectorStat.resize(ipList.size());
            int indx=0;
             for(std::list<Ipv4Address> :: iterator j = ipList.begin(); j!=ipList.end(); j++){
                 
                 if(i!=j){
                    vectorStat(indx) = getSij(*i,*j);
                 } else {
                     vectorStat(indx) = 0;
                 }
                 indx++;
            }

            for(int k = 0; k<vectorStat.size(); k++){
                matrixS(curentRow,k) = vectorStat(k);
            }
            curentRow++;
        }

        std::cout<<"--------------------------------"<<std::endl;
        std::cout<<matrixS<<std::endl;
        std::cout<<"--------------------------------"<<std::endl;
    }

    bool
    Reputation::hasInList(std::list<Ipv4Address> list, Ipv4Address ip){

        for(std::list<Ipv4Address> :: iterator it= list.begin(); it!=list.end(); it++){
            if(*it==ip){
                return true;
            }
        }
        return false;
    }

    int
    Reputation::getSij(Ipv4Address i, Ipv4Address j){

        std::pair<int, int> pair = tableMapRep[i][j];
        return pair.second - (pair.first - pair.second);
    }

    void 
    Reputation::calculateMatrixC(){

    }
    void
    Reputation::calculateReputationTable(){
        calculateMatrixS();
    }
} // namespace ns3