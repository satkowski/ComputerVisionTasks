#include <filter.h>

Mat meanFilter(int w, Mat* input) {
    //Show the original image
    if(w == 0)
        return *input;

    /*------------- initialization ------------*/
    int i, j, wArea;
    Mat temp, output;
    Mat areas(input->rows + 2*w, input->cols + 2*w, CV_64FC3);
    /*-----------------------------------------*/

    wArea = (2*w + 1)*(2*w +1);

    //Convert the image
    input->convertTo(temp, CV_64FC3);

    //Fill the first row and column with 0
    for(int cY = 0; cY <= input->rows; cY++)
        areas.at<Vec3d>(cY, 0) = 0;
    for(int cX = 0; cX <= input->cols; cX++)
        areas.at<Vec3d>(0, cX) = 0;

    //Calculate the "integral" of the window from (0,0) to the pixel
    for(int cY = 0; cY < (input->rows + 2*w); cY++) {
        //Calculate the mirror coordinate
        if(cY - w <= 0)                 j = (cY - w) * - 1;
        else if(cY - w >= input->rows)  j = (cY - w) - ((cY - w) - input->rows + 1);
        else                            j = (cY - w);

        for(int cX = 0; cX < (input->cols + 2*w); cX++) {
            //Calculate the mirror coordinate
            if(cX - w <= 0)                 i = (cX - w) * - 1;
            else if(cX - w >= input->cols)  i = (cX - w) - ((cX - w) - input->cols + 1);
            else                            i = (cX - w);

            //Add the color of the actual pixel
            areas.at<Vec3d>(cY, cX) = temp.at<Vec3d>(j, i);

            //Add the sum of the areas before if there are any
            if(cY - 1 >= 0 && cX - 1 >= 0)
                areas.at<Vec3d>(cY, cX) += areas.at<Vec3d>(cY - 1, cX) +
                                           areas.at<Vec3d>(cY, cX - 1) -
                                           areas.at<Vec3d>(cY  - 1, cX - 1);
            else if(cY  - 1 < 0 && cX  - 1 >= 0)
                areas.at<Vec3d>(cY, cX) += areas.at<Vec3d>(cY, cX - 1);
            else if(cY  - 1 >= 0 && cX  - 1 < 0)
                areas.at<Vec3d>(cY, cX) += areas.at<Vec3d>(cY - 1, cX);
        }
    }

    //Calculate the mean for every pixel with the window size
    for(int cY = 0; cY < input->rows; cY++) {
        //Calculate the center of the window
        j = cY + w;
        for(int cX = 0; cX < input->cols; cX++) {
            //Calculate the center of the window
            i = cX + w;

            //Calculate the sum of the window
            temp.at<Vec3d>(cY, cX) = areas.at<Vec3d>(j + w, i + w);
            if((j - w - 1) >= 0 && (i - w - 1) >= 0)
                temp.at<Vec3d>(cY, cX) = temp.at<Vec3d>(cY, cX) -
                                         areas.at<Vec3d>(j + w, i - w - 1) -
                                         areas.at<Vec3d>(j - w - 1, i + w) +
                                         areas.at<Vec3d>(j - w - 1, i - w - 1);
            else if((j - w - 1) < 0 && (i - w - 1) >= 0)
                temp.at<Vec3d>(cY, cX) -= areas.at<Vec3d>(j + w, i - w - 1);
            else if((j - w - 1) >= 0 && (i - w - 1) < 0)
                temp.at<Vec3d>(cY, cX) -= areas.at<Vec3d>(j - w - 1, i + w);
            //Calculate the mean of the window
            temp.at<Vec3d>(cY, cX) /= wArea;
        }
    }

    //Convert the image back
    temp.convertTo(output, CV_8UC3);    
    return output;
}

Mat medianFilterOneSet(int w, Mat *input) {
    //Show the original image
    if(w == 0)
        return *input;

    /*------------- initialization ------------*/
    Mat tempInput, tempOutput, output;
    Vec3dRGBSet oldRowPixelSet, actualpixelSet;
    std::_Rb_tree_const_iterator<Vec3d> median;
    /*-----------------------------------------*/

    input->convertTo(tempInput, CV_64FC3);
    tempOutput = Mat(tempInput.rows - 2*w, tempInput.cols - 2*w, CV_64FC3);

    for(int wY = 0; wY < 2*w + 1; wY++) {
        for(int wX = 0; wX < 2*w + 1; wX++)
          actualpixelSet.insert(tempInput.at<Vec3d>(wY, wX));
    }
    oldRowPixelSet = actualpixelSet;

    auto begin = std::chrono::high_resolution_clock::now();
    for(int cY = w; cY < tempInput.rows - w; cY++) {
        for(int wX = -w; wX <= w && cY != w; wX++) {
            oldRowPixelSet.erase(oldRowPixelSet.find(tempInput.at<Vec3d>(cY - w - 1, w + wX)));
            oldRowPixelSet.insert(tempInput.at<Vec3d>(cY + w, w + wX));
        }
        actualpixelSet = oldRowPixelSet;

        median = actualpixelSet.begin();
        std::advance(median, actualpixelSet.size() / 2);
        tempOutput.at<Vec3d>(cY - w, 0) = *median;

        for(int cX = w + 1; cX < tempInput.cols - w; cX++) {
            for(int wY = -w; wY <= w; wY++) {
                actualpixelSet.erase(actualpixelSet.find(tempInput.at<Vec3d>(cY + wY, cX - w - 1)));
                actualpixelSet.insert(tempInput.at<Vec3d>(cY + wY, cX + w));
            }
            median = actualpixelSet.begin();
            std::advance(median, actualpixelSet.size() / 2);
            tempOutput.at<Vec3d>(cY - w, cX - w) = *median;
        }
        auto elapsed = std::chrono::high_resolution_clock::now() - begin;
        long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        std::cout << "Time at " << cY << ":    " << double(microseconds) / 1000000 << std::endl;
    }
    tempOutput.convertTo(output, CV_8UC3);
    return output;
}

Mat medianFilterTwoSets(int w, Mat *input) {
    //Show the original image
    if(w == 0)
        return *input;

    /*------------- initialization ------------*/
    Mat tempInput, tempOutput, output;
    unsigned int diff;
    std::_Rb_tree_const_iterator<Vec3d> elemIter;
    Vec3dRGBSet lowerHalf, upperHalf;
    Vec3dRGBSet oldRowLowerHalf, oldRowUpperHalf;
    Vec3d element, actualPixel, median;
    compareRGB compare;
    /*-----------------------------------------*/

    //Convert the image to an image with double pixels
    input->convertTo(tempInput, CV_64FC3);
    tempOutput = Mat(tempInput.rows - 2*w, tempInput.cols - 2*w, CV_64FC3);

    //Fill the multisets for the first time
    for(int wY = 0; wY < 2*w + 1; wY++) {
        for(int wX = 0; wX < 2*w + 1; wX++) {
            lowerHalf.insert(tempInput.at<Vec3d>(wY, wX));
        }
    }
    oldRowLowerHalf = lowerHalf;

    auto begin = std::chrono::high_resolution_clock::now();
    for(int cY = w; cY < tempInput.rows - w; cY++) {
        //Add and delte Pixel
        for(int wX = -w; wX <= w && cY != w; wX++) {
            median = *(--oldRowLowerHalf.end());
            //Delete old Pixel
            actualPixel = tempInput.at<Vec3d>(cY - w - 1, w + wX);
            elemIter = oldRowLowerHalf.find(actualPixel);
            if(*elemIter != *oldRowLowerHalf.end())
                oldRowLowerHalf.erase(elemIter);
            else
                oldRowUpperHalf.erase(oldRowUpperHalf.find(actualPixel));

            //Insert new pixel
            actualPixel = tempInput.at<Vec3d>(cY + w, w + wX);
            if(compare.operator ()(actualPixel, element))
                oldRowLowerHalf.insert(actualPixel);
            else
                oldRowUpperHalf.insert(actualPixel);
        }
        //Find the smaller and bigger set
        if(oldRowLowerHalf.size() - 1 < oldRowUpperHalf.size()) {
            diff = (oldRowUpperHalf.size() - (oldRowLowerHalf.size() - 1)) / 2;
            //Rearenge both multisets
            for(unsigned int i = 0; i < diff; i++) {
                element = *oldRowUpperHalf.begin();
                oldRowLowerHalf.insert(element);
                oldRowUpperHalf.erase(oldRowUpperHalf.find(element));
            }
        }
        else if(oldRowLowerHalf.size() - 1 > oldRowUpperHalf.size()) {
            diff = ((oldRowLowerHalf.size() - 1) - oldRowUpperHalf.size()) / 2;
            //Rearenge both multisets
            for(unsigned int i = 0; i < diff; i++) {
                element = *(--oldRowLowerHalf.end());
                oldRowUpperHalf.insert(element);
                oldRowLowerHalf.erase(oldRowLowerHalf.find(element));
            }
        }
        lowerHalf = oldRowLowerHalf;
        upperHalf = oldRowUpperHalf;
        //Add the median to the ouptput
        median = *(--lowerHalf.end());
        tempOutput.at<Vec3d>(cY - w, 0) = median;

        for(int cX = w + 1; cX < tempInput.cols - w; cX++) {
            //Add and delte Pixel
            for(int wY = -w; wY <= w; wY++) {
                median = *(--lowerHalf.end());
                //Delete old Pixel
                actualPixel = tempInput.at<Vec3d>(cY - wY, cX - w - 1);
                elemIter = lowerHalf.find(actualPixel);
                if(*elemIter != *lowerHalf.end())
                    lowerHalf.erase(elemIter);
                else
                    upperHalf.erase(upperHalf.find(actualPixel));

                //Insert new pixel
                actualPixel = tempInput.at<Vec3d>(cY + wY, cX + w);
                if(compare.operator ()(actualPixel, median))
                    lowerHalf.insert(actualPixel);
                else
                    upperHalf.insert(actualPixel);
            }
            //Find the smaller and bigger set
            if(lowerHalf.size() - 1 < upperHalf.size()) {
                diff = (upperHalf.size() - (lowerHalf.size() - 1)) / 2;
                //Rearenge both multisets
                for(unsigned int i = 0; i < diff; i++) {
                    element = *upperHalf.begin();
                    lowerHalf.insert(element);
                    upperHalf.erase(upperHalf.find(element));
                }
            }
            else if(lowerHalf.size() - 1 > upperHalf.size()) {
                diff = ((lowerHalf.size() - 1) - upperHalf.size()) / 2;
                //Rearenge both multisets
                for(unsigned int i = 0; i < diff; i++) {
                    element = *(--lowerHalf.end());
                    upperHalf.insert(element);
                    lowerHalf.erase(lowerHalf.find(element));
                }
            }
            //Add the median to the ouptput
            median = *(--lowerHalf.end());
            tempOutput.at<Vec3d>(cY - w, cX - w) = median;
//            auto elapsed = std::chrono::high_resolution_clock::now() - begin;
//            long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
//            std::cout << "Time at " << cY << "," << cX << ":    " << double(microseconds) / 1000000 << std::endl;
        }
        auto elapsed = std::chrono::high_resolution_clock::now() - begin;
        long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
        std::cout << "Time at " << cY << ":    " << double(microseconds) / 1000000 << std::endl;
//        std::cout << "-------------------" << std::endl;
    }
    tempOutput.convertTo(output, CV_8UC3);
    return output;
}

Mat sobelFilter(Mat *input) {
    /*------------- initialization ------------*/
    Mat horizontalSobelX, verticalSobelX, horizontalSobelY, verticalSobelY;
    Mat tempInput, output;
    Mat actualWindow, actualPixelX, actualPixelY;
    double actualPixel;
    /*-----------------------------------------*/

    horizontalSobelX = (Mat_<double>(1, 3) << -.5, 0, .5);
    verticalSobelX = (Mat_<double>(3, 1) << .25, .5, .25);
    horizontalSobelY = (Mat_<double>(1, 3) << .25, .5, .25);
    verticalSobelY = (Mat_<double>(3, 1) << -.5, 0, .5);
    actualPixelX = Mat_<double>(1, 1);

    //Converts the image into a b/w one
    cvtColor(*input, tempInput, CV_BGR2GRAY);
    output = Mat_<uchar>(tempInput.rows - 2, tempInput.cols - 2);

    //Iterate over all pixel in the image
    for(int cY = 1; cY < tempInput.rows - 1; cY++) {
        for(int cX = 1; cX < tempInput.cols - 1; cX++) {
            //Get the pixel around the actual one
            actualWindow = (Mat_<double>(3, 3) <<
                           tempInput.at<uchar>(cY - 1, cX - 1), tempInput.at<uchar>(cY - 1, cX), tempInput.at<uchar>(cY - 1, cX + 1),
                           tempInput.at<uchar>(cY    , cX - 1), tempInput.at<uchar>(cY    , cX), tempInput.at<uchar>(cY    , cX + 1),
                           tempInput.at<uchar>(cY + 1, cX - 1), tempInput.at<uchar>(cY + 1, cX), tempInput.at<uchar>(cY + 1, cX + 1));
            //Multiply that pixel with the sobel vectors for the x and y gradiant
            actualPixelX = horizontalSobelX * (actualWindow * verticalSobelX);
            actualPixelY = horizontalSobelY * (actualWindow * verticalSobelY);
            //Combine both gradiant
            actualPixel = sqrt(actualPixelX.at<double>(0, 0) * actualPixelX.at<double>(0, 0) +
                               actualPixelY.at<double>(0, 0) * actualPixelY.at<double>(0, 0));
            //Add the new pixel value to the output
            output.at<uchar>(cY - 1, cX - 1) = actualPixel;
        }
    }
    return output;
}
