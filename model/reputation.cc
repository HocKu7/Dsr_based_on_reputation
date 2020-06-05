#include "reputation.h"

using namespace std;
namespace ns3
{
    Reputation::Reputation(Ipv4Address addr)
    {
        m_mainIp = addr;
        delta = 0.01;
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
        
        //std::cout<<"calculated on: "<<m_mainIp <<std::endl;
        std::list <Ipv4Address> ipList;

        printMap();
        int positionInVector = 0;
        for(std::map<Ipv4Address, std::map<Ipv4Address, std::pair<int, int> > > ::const_iterator nodeEntryMap = tableMapRep.begin();
        nodeEntryMap!=tableMapRep.end(); nodeEntryMap++){

            Ipv4Address i_node = nodeEntryMap->first;
                if(!hasInList(ipList, i_node)){
                    ipList.push_back(i_node);
                    ipToLineMap[i_node] = positionInVector;
                    positionInVector++;
                }
            std::map<Ipv4Address, std::pair<int, int> > nodeMap = nodeEntryMap->second;
            
            for(std::map<Ipv4Address, std::pair<int, int> > :: iterator it = nodeMap.begin(); it!=nodeMap.end();
            it++){

                Ipv4Address j_node = it->first;
                
                if(!hasInList(ipList, j_node)){
                    ipList.push_back(j_node);
                    ipToLineMap[j_node] = positionInVector;
                    positionInVector++;
                }
            }
        }

        matrixS.resize(ipList.size(), ipList.size());
        matrixC.resize(ipList.size(), ipList.size());
        int curentRow=0;
        for(std::list<Ipv4Address> :: iterator i = ipList.begin(); i!=ipList.end(); i++){
            Eigen::VectorXd vectorStat;
            vectorStat.resize(ipList.size());
            int indx=0;
             for(std::list<Ipv4Address> :: iterator j = ipList.begin(); j!=ipList.end(); j++){
                 
                 if(i!=j){
                    vectorStat(indx) = getSij(*i,*j);
                 } 
                 else {
                     vectorStat(indx) = 0;
                 }
                 indx++;
            }
            //std::cout<<vectorStat<<std::endl;
            for(int k = 0; k<vectorStat.size(); k++){
                matrixS(curentRow,k) = vectorStat(k);
            }
            curentRow++;
        }

        countRow = ipList.size();
        countCol = ipList.size();
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
        //std::cout<<"i: "<<i<<" j: "<<j<<" "<<pair.first<<"/"<<pair.second<<std::endl;
        return pair.second - (pair.first - pair.second);
    }

    void 
    Reputation::calculateMatrixC(){
        

        for(int i = 0; i < countRow; i++){

            int sumRow = 0;
            for(int j = 0; j < countCol; j++){
                if(matrixS(i, j) > 0){
                    sumRow += matrixS(i,j);
                }
            
            for(int k = 0; k < countCol; k++){
                // if(i==k){
                //     matrixC(i,k)=1;
                // }else 
                if(matrixS(i, k) > 0){
                    matrixC(i,k) = matrixS(i,k)/sumRow;
                }else{
                    matrixC(i,k) = 0;
                }
                }
            }
        }
    }

    void
    Reputation::calculateVectorT(){

        Eigen::VectorXd res(countCol);
        for(int i=0;i<countCol;i++){
            res(i) = 1.0/countCol;
        }
        Eigen::VectorXd tmp(countCol);
        double norm = 0;
        matrixC.transposeInPlace();
        //std::cout<<"--MatrixC-----------"<<std::endl;
        //std::cout<<matrixC<<std::endl;
        do{
            
            vectorRep = res;
            
            res = matrixC * vectorRep;

            //std::cout<<"--res-----------"<<res<<std::endl;
            //std::cout<<"---vectorRep---------"<<vectorRep<<std::endl;

            tmp = res - vectorRep;
            
            norm = tmp.norm();
            //res.normalize();

            //std::cout<<"---Norm tmp---------"<<norm<<std::endl;
        } while(norm > delta);

        vectorRep = res;
        vectorRep.normalize();
        std::cout<<"---RESULT---------"<<norm<<std::endl;
    }

    void
    Reputation::calculateReputationTable(){

        calculateMatrixS();
        std::cout<<"matrixS done"<<std::endl;
        calculateMatrixC();
        std::cout<<"matrixC done"<<std::endl;
        //std::cout<<matrixC<<std::endl;
        //std::cout<<"--------------"<<std::endl;
        vectorRep.resize(countCol);
        // for(int i=0;i<countCol;i++){
        //     vectorRep(i) = 0.5;
        // }
        calculateVectorT();
        std::cout<<"vectorT done"<<std::endl;
        vectorToMap();
        std::cout<<"vectorToMap done"<<std::endl;
        printResult();
    }

    void 
    Reputation::printMap(){
        
        std::cout<<"--------------"<<std::endl;
          for(std::map<Ipv4Address, std::map<Ipv4Address, std::pair<int, int> > > ::const_iterator nodeEntryMap = tableMapRep.begin();
        nodeEntryMap!=tableMapRep.end(); nodeEntryMap++){
            
            std::cout<<"info from: "<<nodeEntryMap->first<<std::endl;
            std::map<Ipv4Address, std::pair<int, int> > map = nodeEntryMap->second;
            for(std::map<Ipv4Address, std::pair<int, int> > :: iterator it =map.begin(); it!=map.end();
                it++){
                    std::cout<<"Node: "<<it->first<<" : "<<it->second.first<<"/"<<it->second.second<<std::endl;
            }
        }
         std::cout<<"--------------"<<std::endl;
    }

    void 
    Reputation::printResult(){
        
        std::cout<<"-----RESULT------  "<<m_mainIp<<std::endl;
        for(std::map<Ipv4Address, double> ::const_iterator entry = resultMapReputation.begin();
        entry!=resultMapReputation.end(); entry++){
            std::cout<<entry->first<<":"<<entry->second<<std::endl;
        }
         std::cout<<"------RESULT done--------"<<std::endl;
    }

    void 
    Reputation::vectorToMap(){

        for(std::map<Ipv4Address,  int> ::const_iterator entry = ipToLineMap.begin();
            entry!=ipToLineMap.end(); entry++){

                int numberPos = entry->second;
                double valueRep = vectorRep(numberPos);
                resultMapReputation[entry->first] = valueRep;
        }
    }

    std::map<Ipv4Address, double>
    Reputation::getResult(){
        return resultMapReputation;
    }

} // namespace ns3