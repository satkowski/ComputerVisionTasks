#include <filter.h>


Mat sharpening(Mat* input) {
    /*------------- initialization ------------*/
    Mat unsharpMask;
    Mat tempInput, tempOutput, output;
    Vec3d actualValue;
    /*-----------------------------------------*/
    input->convertTo(tempInput, CV_64FC3);
    unsharpMask = (Mat_<double>(3, 3) << -1, -2,  -1,
                                         -2,  12, -2,
                                         -1, -2,  -1);
    tempOutput = Mat(tempInput.rows, tempInput.cols, CV_64FC3);

    for(int cY = 1; cY < tempInput.rows - 1; cY++) {
        for(int cX = 1; cX < tempInput.cols - 1; cX++) {
            actualValue = 0;
            for(int wY = -1; wY <= 1; wY++)
                for(int wX = -1; wX <= 1; wX++)
                    tempOutput.at<Vec3d>(cY - 1, cX - 1) += unsharpMask.at<double>(wY + 1, wX + 1) *
                                                            tempInput.at<Vec3d>(cY + wY, cX + wX);
            tempOutput.at<Vec3d>(cY - 1, cX - 1) /= 4;
        }
    }
    tempOutput.convertTo(output, CV_8UC3);
    std::cout << output << std::endl;
    output += *input;
    return output;
}


Mat medianThreshold(int w, int threshold, Mat* input) {
    //Show the original image
    if(w == 0)
        return *input;

    /*------------- initialization ------------*/
    Mat tempInput, tempOutput, output;
    orderedSet oldRowPixelSet, actualpixelSet;
    std::_Rb_tree_const_iterator<float> median;
    bool overThreshold = false;
    /*-----------------------------------------*/

    cvtColor(*input, tempInput, CV_BGR2GRAY);
    tempInput.convertTo(tempInput, CV_32F);
    tempOutput = Mat(tempInput.rows - 2*w, tempInput.cols - 2*w, CV_32F);

    //Create the first window
    for(int wY = 0; wY < 2*w + 1; wY++) {
        for(int wX = 0; wX < 2*w + 1; wX++) {
          actualpixelSet.insert(tempInput.at<float>(wY, wX));
          if(tempInput.at<float>(wY, wX) > threshold) overThreshold = true;
        }
    }
    oldRowPixelSet = actualpixelSet;

    //Iterate over the whole image
    for(int cY = w; cY < tempInput.rows - w; cY++) {
        //Change the set for the new line
        for(int wX = -w; wX <= w && cY != w; wX++) {
            oldRowPixelSet.erase(oldRowPixelSet.find(tempInput.at<float>(cY - w - 1, w + wX)));
            oldRowPixelSet.insert(tempInput.at<float>(cY + w, w + wX));
            if(tempInput.at<float>(cY + w, w + wX) > threshold) overThreshold = true;
        }
        actualpixelSet = oldRowPixelSet;

        //Save the median in the output image if a value is bigger than the threhold
        if(overThreshold) {
            median = actualpixelSet.begin();
            std::advance(median, actualpixelSet.size() / 2);
            tempOutput.at<float>(cY - w, 0) = *median;
            overThreshold = false;
        }

        for(int cX = w + 1; cX < tempInput.cols - w; cX++) {
            //Change the set on the same line but new column
            for(int wY = -w; wY <= w; wY++) {
                actualpixelSet.erase(actualpixelSet.find(tempInput.at<float>(cY + wY, cX - w - 1)));
                actualpixelSet.insert(tempInput.at<float>(cY + wY, cX + w));
                if(tempInput.at<float>(cY + wY, cX + w) > threshold) overThreshold = true;
            }
            //Save the median in the output image if a value is bigger than the threhold
            if(overThreshold) {
                median = actualpixelSet.begin();
                std::advance(median, actualpixelSet.size() / 2);
                tempOutput.at<float>(cY - w, cX - w) = *median;
                overThreshold = false;
            }
        }
    }
    tempOutput.convertTo(output, CV_8U);
    return output;
}
