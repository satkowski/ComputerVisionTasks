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
             "{task     |1  |0                               }";

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
    namedWindow("Output Image", 0);

    setTask(task, trackBarValue, data, image, outputImage);

    waitKey();
    return 0;

}

void setTask(int& task, int& trackBarValue, void* data, Mat& image, Mat& outputImage) {
    createTrackbar("Window Size", "Output Image", &trackBarValue, 582, taskSelect, data);

    //Show the image
    imshow("Output Image", outputImage);
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

    imshow("Output Image", logTrans(taskBarValue, image));
//    namedWindow("Original Image2", 0);
//    imshow("Output Image2", powerLawTrans(taskBarValue, image));
//    namedWindow("Original Image3", 0);
//    imshow("Output Image3", myChoiceTrans(taskBarValue, image));
}
