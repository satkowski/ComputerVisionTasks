#ifndef MULTISET_H
#define MULTISET_H

#include "opencv2/imgproc/imgproc.hpp"

#include <set>

using namespace std;
using namespace cv;

//Struct for the comparsion in the multiset
struct compValue {
    //Using own comparsion method for the Vec3d
    bool operator()(Vec3d left, Vec3d right) const;
};

typedef std::multiset<Vec3d, compValue> Vec3dSet;
typedef Vec<Vec3dSet, 2> Vec2Set;

#endif // MULTISET_H
