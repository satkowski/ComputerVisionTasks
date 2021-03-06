#ifndef FILTER_H
#define FILTER_H

#include "opencv2/imgproc/imgproc.hpp"

#include <multiset.h>
#include <chrono>

#include <iostream>

using namespace std;
using namespace cv;

//Method for the mean filter
Mat meanFilter(int w, Mat* input);
//Method for the median filter
Mat medianFilterOneSet(int w, Mat* input);
//Method for the median filter
Mat medianFilterTwoSets(int w, Mat* input);
//Method for the sobel filter
Mat sobelFilter(Mat* input);

#endif // FILTER_H
