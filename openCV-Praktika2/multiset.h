#ifndef MULTISET_H
#define MULTISET_H

#include "opencv2/imgproc/imgproc.hpp"

#include <set>

using namespace std;
using namespace cv;

//Struct for the comparsion in the multiset for RGB values
struct comparing {
    //Using own comparsion method
    bool operator()(const float& left,const float& right) const;
};


typedef std::multiset<float, comparing> orderedSet;

#endif // MULTISET_H
