#include <main.h>

int main( int argc, const char** argv ) {
    /*------------- initialization ------------*/
    String filename;
    int task, trackBarValue = 0;
    Mat image, outputImage;
    Vec<void*, 2>* data;
    /*-----------------------------------------*/

    const char* keyMap;
    //Standard image that will be used if dont exist arguments
    keyMap = "{path     |   |../Bilder/haus.jpg }"
             "{task     |1  |2                               }";

    //Reading the Callingarguments
    CommandLineParser parser(argc, argv, keyMap);
    filename = parser.get<String>("path");
    task = parser.get<int>("task");

    //Creating the image and testing if it is empty or not
    image = imread(filename);
    if(image.empty()) {
        printf("Cannot read the image %s\n", filename.c_str());
        return -1;
    }
    image.copyTo(outputImage);

    //Filling the data pointer
    data->val[0] = &task;
    data->val[1] = &image;

    //Creating windows for the images
    namedWindow("Original Image", 0);
    imshow("Original Image", image);

    setTask(task, trackBarValue, data, image, outputImage);

    waitKey();
    return 0;

}

void setTask(int& task, int& trackBarValue, void* data, Mat& image, Mat& outputImage) {
    /*------------- initialization ------------*/
    vector<Mat> outputImages;
    /*-----------------------------------------*/
    switch(task) {
    case 1:
        outputImages = histogrammEqualizationOneChannel(&image);
        //Show the images
        imshow("CDF Spine", outputImages.at(0));
        imshow("CDF input", outputImages.at(1));
        imshow("Spine image", outputImages.at(2));
        imshow("Equalization with pdf", outputImages.at(3));
        imshow("Equalization with cdf", outputImages.at(4));
        break;
    case 2:
        outputImages = histogrammEqualizationThreeChannel(&image, false);
        imshow("CDF Rock B", outputImages.at(0));
        imshow("CDF input B", outputImages.at(1));
        imshow("CDF Rock G", outputImages.at(2));
        imshow("CDF input G", outputImages.at(3));
        imshow("CDF Rock R", outputImages.at(4));
        imshow("CDF input R", outputImages.at(5));
        imshow("Rock image", outputImages.at(6));
        imshow("Equalization with cdf on all channel", outputImages.at(7));
        break;
    case 3:
        outputImages = histogrammEqualizationThreeChannel(&image, true);
        break;
    case 4:
        outputImage = sharpening(&image);
        imshow("Unsharp mask", outputImage);
        break;
    default:
        namedWindow("Output image");
        createTrackbar("Window Size", "Output Image", &trackBarValue, 582, taskSelect, data);
        //Show the image
        imshow("Output Image", outputImage);
        break;
    }
}

void taskSelect(int taskBarValue, void* userdata) {
    /*------------- initialization ------------*/
    Vec<void*, 3>* data;
    int* task;
    Mat* image;
    /*-----------------------------------------*/
    //Cast all variables
    data = static_cast<Vec<void*, 3>*>(userdata);
    task = static_cast<int*>(data->val[0]);
    image = static_cast<Mat*>(data->val[1]);

    switch (*task) {
    case 0:
        imshow("Output Image", logTrans(taskBarValue, image));
    //    namedWindow("Original Image2", 0);
    //    imshow("Output Image2", powerLawTrans(taskBarValue, image));
    //    namedWindow("Original Image3", 0);
    //    imshow("Output Image3", myChoiceTrans(taskBarValue, image));
        break;
    case 5:
        imshow("Output Image", medianThreshold(taskBarValue, image));
        break;
    }
}
