#ifndef FILTER_H
#define FILTER_H

#include "opencv2/imgproc/imgproc.hpp"

#include <multiset.h>

using namespace std;
using namespace cv;

//Method for the mean filter
void meanFilter(int w, Mat* input);

//Method for the median filter
void medianFilter(int w, Mat* input);

#endif // FILTER_H
