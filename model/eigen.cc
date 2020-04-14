#include "eigen.h"

namespace ns3 {
Eigen::Eigen(){
    testField="Helloy from lib!";
}

string
Eigen::getTestField(){

     return testField;
}
}