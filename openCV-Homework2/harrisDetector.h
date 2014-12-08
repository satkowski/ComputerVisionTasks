#ifndef HARRISDETECTOR_H
#define HARRISDETECTOR_H

#include "opencv2/imgproc/imgproc.hpp"

#include <multiset.h>
#include <chrono>

#include <iostream>

#define k 0.04
#define PI 3.14159265

//Method for the harris corner detector
Mat harrisCornerDetector(int w, int gRange, int threshholding, Mat* input);

//Thresholding with non-max-suppresion
Mat windowSizeThreshold(int& w, Mat& harrisValueMat, Mat* input);
//Thresholding with percentage of the biggest harris values
Mat percentageThreshold(int& w, Vec3dHarrisSet& interesstPoints, Mat* input);

#endif // HARRISDETECTOR_H
