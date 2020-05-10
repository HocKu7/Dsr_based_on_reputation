#ifndef EIGEN_H
#define EIGEN_H

#include <string>
#include "/home/vanet/tarballs/EigenLib/eigen-3.2.10/Eigen/Dense"

using namespace std;
namespace ns3
{
    class Eigen
    {

    private:
        string testField;

    public:
        Eigen();
        string getTestField();
    };
} // namespace ns3
#endif /* EIGEN_H */