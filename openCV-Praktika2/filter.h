#ifndef FILTER_H
#define FILTER_H

#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;

Mat sharpening(Mat* input);
Mat medianThreshold(int threshold, Mat* input);

#endif // FILTER_H
