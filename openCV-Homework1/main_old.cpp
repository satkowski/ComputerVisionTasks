#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>

using namespace cv;

Mat image, output;
Vec3s black, white, red;
Vec3s intensity; //The same like Vec<int, 3>
int distanceThreshhold, reset;

//Method that will be called if the Trackbar is used
static void onTrackbar(int, void*) {
    Vec3s newIntesity;
    int distance;
    for(int cY = 0; cY < output.rows; cY++) {
        for(int cX = 0; cX < output.cols; cX++) {
            newIntesity = output.at<Vec3s>(cY, cX);
            distance = sqrt(
                        pow((intensity.val[0] - newIntesity.val[0]), 2) +
                        pow((intensity.val[1] - newIntesity.val[1]), 2) +
                        pow((intensity.val[2] - newIntesity.val[2]), 2));
            printf("x: %d, y: %d --- %d\n", cY, cX, distance);

            if(distance < distanceThreshhold) {
                output.at<Vec3s>(cY, cX) = black;
            }
            else {
                output.at<Vec3s>(cY, cX) = white;
            }
        }
    }
    imshow("Image", output);
}

static void onResetTrackbar(int, void*) {
    if(reset == 1) {
        imshow("Image", image);
        reset = 0;
    }
}

//Method that will be called if the Mouse is moved and clicked
void mouseListener(int event, int x, int y, int flags, void* userdata) {
    if(event == EVENT_LBUTTONDBLCLK) {
        intensity = image.at<Vec3s>(y, x);
        onTrackbar(distanceThreshhold, 0);
    }
}

int main( int argc, const char** argv ) {
    black = {0, 0, 0};
    white = {255, 255, 255};
    red = {0, 0, 255};

    String filename;
    const char* keyMap;

    //Standard image that will be used if dont exist arguments
    keyMap = "{1||prep-for-grilling.jpg}";

    //Reading the Callingarguments
    CommandLineParser parser(argc, argv, keyMap);
    filename = parser.get<String>("1");

    //Creating the image and testing if it is empty or not
    image = imread(filename);
    if(image.empty()) {
        printf("Cannot read the image %s\n", filename.c_str());
        return -1;
    }
    image.copyTo(output);

    //Creating of the inputimage window
    namedWindow("Image", 0);
    imshow("Image", image);

    //Adding the trackbar to the outputwindow
    createTrackbar("Farbabstand", "Image", &distanceThreshhold, 100, onTrackbar);

    //Adding mouselistener to the inputwindow
    setMouseCallback("Image", mouseListener);

    //Adding reset trackbar as a button
    createTrackbar("Reset", "Image", &reset, 1, onResetTrackbar);




    waitKey();
    return 0;
}
