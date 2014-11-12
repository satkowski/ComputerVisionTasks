#ifndef FILTER_H
#define FILTER_H

#include "opencv2/imgproc/imgproc.hpp"

#include <set>

using namespace cv;

typedef Vec<std::multiset<Vec3d>, 2> Vec2Set;
typedef Matx<Vec2Set, 2, 2> MatSet;

//Method for the mean filter
void meanFilter(int w, Mat* input);

//Method for the median filter
void medianFilter(int w, Mat* input);

#endif // FILTER_H
