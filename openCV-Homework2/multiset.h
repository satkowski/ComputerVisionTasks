#ifndef MULTISET_H
#define MULTISET_H

#include "opencv2/imgproc/imgproc.hpp"

#include <set>

using namespace std;
using namespace cv;

//Struct for the comparsion in the multiset for RGB values
struct compareRGB {
    //Using own comparsion method
    bool operator()(const Vec3d& left,const Vec3d& right) const;
};

struct compareHarris {
    //Using own comparsion method
    bool operator()(const Vec3d& left,const Vec3d& right) const;
};


typedef std::multiset<Vec3d, compareRGB> Vec3dRGBSet;
typedef std::multiset<Vec3d, compareHarris> Vec3dHarrisSet;

#endif // MULTISET_H
