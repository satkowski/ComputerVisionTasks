#include <harrisDetector.h>

Mat harrisCornerDetector(int w, int threshholding, Mat* input) {
    /*------------- initialization ------------*/
    Mat tempInput;
    Mat partDerivX, partDerivY;
    Mat harrisMat, harrisValueMat;
    double harrisValue;
    Vec3dHarrisSet interesstPoints;
    /*-----------------------------------------*/

    //Convert the image to an image with double pixels
    cvtColor(*input, tempInput, CV_BGR2GRAY);
    harrisValueMat = Mat_<double>(tempInput.rows, tempInput.cols);

    partDerivX = Mat(tempInput.rows, tempInput.cols, CV_64F);
    partDerivY = Mat(tempInput.rows, tempInput.cols, CV_64F);

    //Calculate gradient in x direction
    for(int cY = 0; cY < tempInput.rows; cY++) {
        for(int cX = 1; cX < tempInput.cols - 1; cX++) {
            //( I(x + 1, y) - I(x - 1, y) ) / 2
            partDerivX.at<double>(cY, cX) = (static_cast<double>(tempInput.at<uchar>(cY, cX + 1)) -
                                             static_cast<double>(tempInput.at<uchar>(cY, cX - 1))) / 2;
        }
    }
    //Calculate gradient in y direction
    for(int cX = 0; cX < tempInput.cols; cX++) {
        for(int cY = 1; cY < tempInput.rows - 1; cY++) {
            //( I(x, y + 1) - I(x, y - 1) ) / 2
            partDerivY.at<double>(cY, cX) = (static_cast<double>(tempInput.at<uchar>(cY + 1, cX)) -
                                             static_cast<double>(tempInput.at<uchar>(cY - 1, cX))) / 2;
        }
    }

    //Calculate the interest points
    for(int cY = 1; cY < tempInput.rows - 1; cY++) {
        for(int cX = 1; cX < tempInput.cols - 1; cX++) {
            //Fill the harris matrix
            harrisMat = (Mat_<double>(2, 2) <<
                       partDerivX.at<double>(cY, cX) * partDerivX.at<double>(cY, cX),   //I of x square
                       partDerivX.at<double>(cY, cX) * partDerivY.at<double>(cY, cX),   //I of x times I of y
                       partDerivY.at<double>(cY, cX) * partDerivX.at<double>(cY, cX),   //I of y times I of x
                       partDerivY.at<double>(cY, cX) * partDerivY.at<double>(cY, cX));  //I of y square
            //Calc the harris value
            harrisValue = (harrisMat.at<double>(0, 0) * harrisMat.at<double>(1, 1) -
                           harrisMat.at<double>(0, 1) * harrisMat.at<double>(1, 0)) -   //det(harrisMat)
                           k *
                         ((harrisMat.at<double>(0, 0) + harrisMat.at<double>(1, 1)) *
                          (harrisMat.at<double>(0, 0) + harrisMat.at<double>(1, 1)));   //trace(harrisMat) square
            //Decide which datastructure is used
            if(threshholding == 0)
                //Adding the values to the matrix
                harrisValueMat.at<double>(cY, cX) = abs(harrisValue);
            else// if(threshholding == 1)
                //Adding the points to an multiset that is orderd to the harris values
                interesstPoints.insert(Vec3d { cX, cY,                  //Point of the Pixel
                                               abs(harrisValue) });     //Harris value of the pixel
        }
    }

    //Decide which function is used
    if(threshholding == 0)
        return windowSizeThreshold(w, harrisValueMat, input);
    else// if(threshholding == 1)
        return percentageThreshold(w, interesstPoints, input);
}

Mat windowSizeThreshold(int& w, Mat& harrisValueMat, Mat* input) {
    /*------------- initialization ------------*/
    Mat output;
    double maxHarrisValue;
    Point maxHarrisPoint;
    /*-----------------------------------------*/

    input->copyTo(output);

    //Non max suppresion on the harris value
    for(int cY = w; cY < harrisValueMat.rows - w; cY++) {
        for(int cX = w; cX < harrisValueMat.cols - w; cX++) {
            maxHarrisValue = 0;
            //Iterate through the window and search the biggest harris value
            for(int wY = -w; wY <= w; wY++) {
                for(int wX = -w; wX <= w; wX++) {
                    //Set the new maximum
                    if(maxHarrisValue < harrisValueMat.at<double>(cY + wY, cX + wX)) {
                        maxHarrisValue = harrisValueMat.at<double>(cY + wY, cX + wX);
                        maxHarrisPoint = Point(cX + wX, cY + wY);
                    }
                }
            }
            //Draw the circle in the image
            circle(output, maxHarrisPoint, 2, Scalar(0, 0, 255));
        }
    }
    return output;
}

Mat percentageThreshold(int& w, Vec3dHarrisSet& interesstPoints, Mat* input) {
    /*------------- initialization ------------*/
    Mat output;
    std::_Rb_tree_const_iterator<Vec3d> elemIter;
    /*-----------------------------------------*/

    input->copyTo(output);

    elemIter = interesstPoints.begin();
    //Take only certain number of the biggest harris values
    for(unsigned int i = 0; i < (interesstPoints.size()) / (1 + w); i++) {
        //Draw the circle in the image
        circle(output, Point(elemIter->val[0], elemIter->val[1]), 2, Scalar(0, 0, 255));
        elemIter = (++elemIter);
    }
    return output;
}
