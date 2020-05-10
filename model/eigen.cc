#include "eigen.h"

using namespace std;
namespace ns3
{
    Eigen::Eigen()
    {
        testField = "Helloy from lib!";
    }

    string
    Eigen::getTestField()
    {

        return testField;
    }
} // namespace ns3