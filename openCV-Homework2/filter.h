#ifndef FILTER_H
#define FILTER_H

#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

//Method for the mean filter
void meanFilter(int w, Mat* input);

#endif // FILTER_H
