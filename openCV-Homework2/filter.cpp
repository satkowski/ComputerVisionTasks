#include <filter.h>

Mat meanFilter(int w, Mat* input) {
    //Show the original image
    if(w == 0)
        return *input;

    int i, j, wArea = (2*w + 1)*(2*w +1);
    Mat temp, output;
    Mat areas(input->rows + 2*w, input->cols + 2*w, CV_64FC3);

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

//void medianFilter(int w, Mat* input) {
//    //Show the original image
//    if(w == 0)
//        return;

//    int i, j;
//    Mat tempInput, tempOutput, output;
//    Vec3dSet oldRowPixelSet, actualpixelSet;
//    std::_Rb_tree_const_iterator<Vec3d> median;

//    //Convert the image
//    input->convertTo(tempInput, CV_64FC3);
//    input->convertTo(tempOutput, CV_64FC3);

//    for(int wY = - w; wY < w; wY++) {
//        for(int wX = - w; wX < w; wX++) {
//            actualpixelSet.insert(tempInput.at<Vec3d>(w - abs(wY), w - abs(wX)));
//            //If wX == 0 the same column should be choosed again
//            if(wX == 0)
//                actualpixelSet.insert(tempInput.at<Vec3d>(w - abs(wY), w - abs(wX)));
//            //If wY == 0 the same row should be choosed again
//            if(wY == 0) {
//                actualpixelSet.insert(tempInput.at<Vec3d>(w - abs(wY), w - abs(wX)));
//                if(wX == 0)
//                    actualpixelSet.insert(tempInput.at<Vec3d>(w - abs(wY), w - abs(wX)));
//            }
//        }
//    }

//    for(int cY = 0; cY < input->rows; cY++) {
//        //Add the median of all pixels in the window to the new image
//        median = actualpixelSet.begin();
//        std::advance(median, actualpixelSet.size() / 2);
//        tempOutput.at<Vec3d>(cY, 0) = *median;
//        oldRowPixelSet = actualpixelSet;

//        for(int cX = 1; cX < input->cols; cX++) {
//            //Calculate the mirror coordinate
//            if(cX + w >= input->cols)   i = (cX + w) - ((cX + w) - input->cols + 2);
//            else                        i = (cX + w);

//            //Erase the left pixel of the old window and add the right ones
//            for(int wY = 0; wY <= 2*w; wY++) {
//                //Calculate the mirror coordinate ------- Optimierung an der Stelle!!!
//                if(cY + w - wY <= 0)                j = (cY + w - wY) * -1;
//                else if(cY + w -wY >= input->rows)  j = (cY + w - wY) - ((cY + w - wY) - input->rows + 2);
//                else                                j =  cY + w - wY;

////                Vec3d test1 = tempInput.at<Vec3d>(j, abs(cX - w));
////                std::_Rb_tree_const_iterator<Vec3d> test2 = actualpixelSet.find(tempInput.at<Vec3d>(j, abs(cX - w)));
//                actualpixelSet.erase(actualpixelSet.find(tempInput.at<Vec3d>(j, abs(cX - w - 1))));
//                actualpixelSet.insert(tempInput.at<Vec3d>(j, i));
//            }

//            //Add the median of all pixels in the window to the new image
//            median = actualpixelSet.begin();
//            std::advance(median, actualpixelSet.size() / 2);
////
/// Vec3d test3 = *median;
//            tempOutput.at<Vec3d>(cY, cX) = *median;
//        }
//        if(cY == input->rows - 1)
//            continue;

//        //Calculate the mirror coordinate
//        if(cY + 1 + w >= input->rows)   j = (cY + 1 + w) - ((cY + 1 + w) - input->rows + 2);
//        else                            j = (cY + 1 + w);

//        actualpixelSet = oldRowPixelSet;
//        //Erase the upper pixel of the old window and add the lower ones
//        for(int wX = 0; wX <= 2*w; wX++) {
//            //Calculate the mirror coordinate
//            i = abs(w - wX);

//            actualpixelSet.erase(actualpixelSet.find(tempInput.at<Vec3d>(abs(cY - w), i)));
//            actualpixelSet.insert(tempInput.at<Vec3d>(j, i));
//        }
//    }
//    //Convert the image back
//    tempOutput.convertTo(output, CV_8UC3);
//    *input = output;
//}

Mat medianFilter(int w, Mat *input) {
    //Show the original image
    if(w == 0)
        return *input;

    Mat tempInput, output;
    Vec3dSet oldRowPixelSet, actualpixelSet;
    std::_Rb_tree_const_iterator<Vec3d> median;

    input->convertTo(tempInput, CV_64FC3);
    Mat tempOutput = Mat(tempInput.rows - 2*w, tempInput.cols - 2*w, CV_64FC3);

    for(int wY = 0; wY < 2*w + 1; wY++) {
        for(int wX = 0; wX < 2*w + 1; wX++) {
            actualpixelSet.insert(tempInput.at<Vec3d>(wY, wX));
        }
    }
    oldRowPixelSet = actualpixelSet;

    for(int cY = w; cY < tempInput.rows - w; cY++) {
        for(int wX = 0; wX <= w && cY != w; wX++) {
            oldRowPixelSet.erase(oldRowPixelSet.find(tempInput.at<Vec3d>(cY - w - 1, w + wX)));
            oldRowPixelSet.insert(tempInput.at<Vec3d>(cY + w, w + wX));
            if(wX == 0) continue;
            oldRowPixelSet.erase(oldRowPixelSet.find(tempInput.at<Vec3d>(cY - w - 1, w - wX)));
            oldRowPixelSet.insert(tempInput.at<Vec3d>(cY + w, w - wX));
        }
        actualpixelSet = oldRowPixelSet;

        median = actualpixelSet.begin();
        std::advance(median, actualpixelSet.size() / 2);
        tempOutput.at<Vec3d>(cY - w, 0) = *median;

        for(int cX = w + 1; cX < tempInput.cols - w; cX++) {
            for(int wY = 0; wY <= w; wY++) {
                actualpixelSet.erase(actualpixelSet.find(tempInput.at<Vec3d>(cY + wY, cX - w - 1)));
                actualpixelSet.insert(tempInput.at<Vec3d>(cY + wY, cX + w));
                if(wY == 0) continue;
                actualpixelSet.erase(actualpixelSet.find(tempInput.at<Vec3d>(cY - wY, cX - w - 1)));
                actualpixelSet.insert(tempInput.at<Vec3d>(cY - wY, cX + w));
            }
            median = actualpixelSet.begin();
            std::advance(median, actualpixelSet.size() / 2);
            tempOutput.at<Vec3d>(cY - w, cX - w) = *median;
        }
    }
    tempOutput.convertTo(output, CV_8UC3);
    return output;
}
