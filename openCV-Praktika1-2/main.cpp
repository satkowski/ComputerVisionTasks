#include <main.h>

int main( int argc, const char** argv ) {
    String sourceName;

    Vec3i spherCoord (0,0,0);
    Vec3i affineTrans (0,0,0);
    Point3d camPos;
    Vec<void*, 2> data (&spherCoord, &camPos);

    const char* keyMap;
    //Standard image that will be used if dont exist arguments
    keyMap = "{1       |   |../Bilder/flagge-deutschland.jpg }";

    //Reading the Callingarguments
    CommandLineParser parser(argc, argv, keyMap);
    sourceName = parser.get<String>("1");

    //Creating the images and testing if it is empty or not
    SOURCEIMAGE = imread(sourceName);
    if(SOURCEIMAGE.empty()) {
        printf("Cannot read the image %s\n", sourceName.c_str());
        return -1;
    }

    //Creating the original image window
    namedWindow("Original Image", 0);
    imshow("Original Image", SOURCEIMAGE);

    //Creating the camera window
    namedWindow("Camera Image", 0);

    //Adding the trackbars for the spherical coordinates
    createTrackbar("radius", "Camera Image", &(spherCoord.val[0]), 100, calcCameraPosition, &data);
    createTrackbar("theta ", "Camera Image", &(spherCoord.val[1]), 90, calcCameraPosition, &data);
    createTrackbar("rho   ", "Camera Image", &(spherCoord.val[2]), 360, calcCameraPosition, &data);

    //Adding the trackbars for the affine transformation
    createTrackbar("alpha", "Camera Image", &(affineTrans.val[0]), 100, calcAffineTransformation, &affineTrans);
    createTrackbar("beta ", "Camera Image", &(affineTrans.val[1]), 100, calcAffineTransformation, &affineTrans);
    createTrackbar("lamda", "v", &(affineTrans.val[2]), 100, calcAffineTransformation, &affineTrans);

    waitKey();
    return 0;
}

static void calcCameraPosition(int, void* userdata) {
    Vec<void*, 2>* data;
    Vec3i* spherCoord;
    Point3d* camPos;
    double pointOnXY;

    //Cast of the userdata
    data = static_cast<Vec<void*, 2>*>(userdata);
    spherCoord = static_cast<Vec3i*>(data->val[0]);
    camPos = static_cast<Point3d*>(data->val[1]);

    //Calculate the lengt of the vector on the x-y-plane
    pointOnXY = spherCoord->val[0] * sin(double(spherCoord->val[1]));
    //Calculate the coordinates on the x and y axis with the vector
    camPos->x = pointOnXY * cos(double(spherCoord->val[2]));
    camPos->y = pointOnXY * sin(double(spherCoord->val[2]));
    //Calculate the coordinate on the z axis with theta
    camPos->z = spherCoord->val[0] * cos(double(spherCoord->val[1]));
}

static void calcAffineTransformation(int, void* userdata) {

}
