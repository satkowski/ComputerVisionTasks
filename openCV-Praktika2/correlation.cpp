#include <correlation.h>

Mat crossCorrelation() {
    /*------------- initialization ------------*/
    String filename1 = "../openCV-Praktika2/taskImages/phantomdogs.jpg";
    String filename2 = "../openCV-Praktika2/taskImages/dog.jpg";
    Mat searchImage, templateImage, output;
    Mat tempSearchImage, tempTemplateImage;
//    double averageSearchImage = 0, averageTemplateImage = 0;
    double squaredSumTemplate, squaredSumPatch;
    /*-----------------------------------------*/

    //Creating the image and testing if it is empty or not
    searchImage = imread(filename1);
    while(searchImage.empty()) {
        printf("Cannot read the image %s\n", filename1.c_str());
    }
    templateImage = imread(filename2);
    while(templateImage.empty()) {
        printf("Cannot read the image %s\n", filename2.c_str());
    }
    cvtColor(searchImage, tempSearchImage, CV_BGR2GRAY);
    cvtColor(templateImage, tempTemplateImage, CV_BGR2GRAY);
    tempSearchImage.convertTo(searchImage, CV_64F);
    tempTemplateImage.convertTo(templateImage, CV_64F);

    output = Mat(searchImage.rows - templateImage.rows + 1,
                 searchImage.cols - templateImage.cols + 1,
                 CV_64F);

//    for(int cY = 0; cY < searchImage.rows; cY++) {
//        for(int cX = 0; cX < searchImage.cols; cX++) {
//            averageSearchImage += searchImage.at<double>(cY, cX);
//        }
//    }
//    averageSearchImage /= searchImage.cols * searchImage.rows;
    for(int cY = 0; cY < templateImage.rows; cY++)
        for(int cX = 0; cX < templateImage.cols; cX++)
            squaredSumTemplate += templateImage.at<double>(cY, cX) *
                                  templateImage.at<double>(cY, cX);
//    averageTemplateImage /= templateImage.cols * templateImage.rows;

    for(int cY = 0; cY < output.rows; cY++) {
        for(int cX = 0; cX < output.cols; cX++) {
            output.at<double>(cY, cX) = 0;
            squaredSumPatch = 0;
            //Iterate over the template
            for(int wY = 0; wY < templateImage.rows; wY++) {
                for(int wX = 0; wX < templateImage.cols; wX++) {
                    output.at<double>(cY, cX) += templateImage.at<double>(wY, wX) *
                                                 searchImage.at<double>(cY + wY, cX + wX);
                    squaredSumPatch = searchImage.at<double>(cY + wY, cX + wX) *
                                      searchImage.at<double>(cY + wY, cX + wX);
                }
            }
            output.at<double>(cY, cX) /= sqrt(squaredSumTemplate * squaredSumPatch);
        }
    }
    return output;
}
