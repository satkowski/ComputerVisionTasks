#ifndef FILTER_H
#define FILTER_H

#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <set>

#include <multiset.h>

using namespace cv;

Mat sharpening(Mat* input);
Mat medianThreshold(int w, int threshold, Mat* input);

#endif // FILTER_H
