#include <main.h>

int main( int argc, const char** argv ) {
    String sourceName;
    int xPos, xNeg, yPos, yNeg;

    Vec3i spherCoord (0, 0, 0);
    Vec3i affineTrans (0, 0, 0);
    Point3d camPos;

    Mat rotMat = Mat_<double>(3, 3);
    rotMat.at<double>(0, 2) = 0;
    rotMat.at<double>(1, 2) = 0;
    rotMat.at<double>(2, 0) = 0;
    rotMat.at<double>(2, 1) = 0;
    rotMat.at<double>(2, 2) = 1;

    Vec<void*, 3> dataCamPos (&spherCoord, &camPos, &rotMat);

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

    //Calculate the center of the image
    U0 = SOURCEIMAGE.cols / 2;
    V0 = SOURCEIMAGE.rows / 2;

    //Calculating the new coordinates for the center of the image
    xNeg = 0 - U0;
    xPos = SOURCEIMAGE.cols - U0 - 1;
    if(SOURCEIMAGE.rows % 2 == 1) {
        yNeg = 0 - U0;
        yPos = SOURCEIMAGE.rows - V0 - 1;
    }
    else {
        yNeg = 0 - V0 + 1;
        yPos = SOURCEIMAGE.rows - V0;
    }
    //Add the corners of the source image and
    SOURCEPOINTS.push_back(Point(xNeg, yPos));
    SOURCEPOINTS.push_back(Point(xPos, yPos));
    SOURCEPOINTS.push_back(Point(xNeg, yNeg));
    SOURCEPOINTS.push_back(Point(xPos, yNeg));

    //Creating the original image window
    namedWindow("Original Image", 0);
    imshow("Original Image", SOURCEIMAGE);

    //Creating the camera window
    namedWindow("Camera Image", 0);

    //Adding the trackbars for the spherical coordinates
    createTrackbar("radius", "Camera Image", &(spherCoord.val[0]), 100, calcCameraImage, &dataCamPos);
    createTrackbar("theta ", "Camera Image", &(spherCoord.val[1]), 90, calcCameraImage, &dataCamPos);
    createTrackbar("rho   ", "Camera Image", &(spherCoord.val[2]), 360, calcRotationMatrix, &dataCamPos);

    //Adding the trackbars for the affine transformation
    createTrackbar("alpha", "Camera Image", &(affineTrans.val[0]), 100, calcAffineTransformation, &affineTrans);
    createTrackbar("beta ", "Camera Image", &(affineTrans.val[1]), 100, calcAffineTransformation, &affineTrans);
    createTrackbar("lamda", "Camera Image", &(affineTrans.val[2]), 100, calcAffineTransformation, &affineTrans);

    waitKey();
    return 0;
}

static void calcRotationMatrix(int value, void* userdata) {
    Vec<void*, 3>* data;
    Mat* rotMat;
    double dValue = value;

    //Cast of the userdata
    data = static_cast<Vec<void*, 3>*>(userdata);
    rotMat = static_cast<Mat*>(data->val[2]);

    //Calculation of the new entrys of the rotation matrix
    rotMat->at<double>(0, 0) = cos(dValue);
    rotMat->at<double>(1, 0) = sin(dValue);
    rotMat->at<double>(0, 1) = -rotMat->at<double>(1, 0);
    rotMat->at<double>(1, 1) = -rotMat->at<double>(0, 0);

    //Calling the camera position method
    calcCameraImage(0, userdata);
}

static void calcCameraImage(int, void* userdata) {
    Vec<void*, 3>* data;
    Point3d* camPos;
    Mat* rotMat;

    Mat firstMat = Mat_<double>(3, 3);
    Mat secondMat = Mat_<double>(3, 4);
    Mat point3D = Mat_<double>(4, 1);

    Point actualPoint;
    Mat tempPoint = Mat_<double>(1, 3);
    vector<Point> destinationPoints;

    Mat homography, sourceWarped;

    calcCameraPosition(0, userdata);

    //Cast of the userdata
    data = static_cast<Vec<void*, 3>*>(userdata);
    camPos = static_cast<Point3d*>(data->val[1]);
    rotMat = static_cast<Mat*>(data->val[2]);

    //Fill the first matrix
    firstMat.at<double>(0, 0) = ALPHA;
    firstMat.at<double>(0, 1) = 0;
    firstMat.at<double>(0, 2) = U0;
    firstMat.at<double>(1, 0) = 0;
    firstMat.at<double>(1, 1) = ALPHA;
    firstMat.at<double>(1, 2) = V0;
    firstMat.at<double>(2, 0) = 0;
    firstMat.at<double>(2, 1) = 0;
    firstMat.at<double>(2, 2) = 1;

    //Fill the second matrix
    for(int cY = 0; cY < 3; cY++) {
        for(int cX = 0; cX < 3; cX++)
            secondMat.at<double>(cY, cX) = rotMat->at<double>(cY, cX);
    }
    secondMat.at<double>(0, 3) = camPos->x;
    secondMat.at<double>(1, 3) = camPos->y;
    secondMat.at<double>(2, 3) = camPos->z;

    //Fill the point vector from the image
    point3D.at<double>(0, 2) = 0;
    point3D.at<double>(0, 3) = 1;

    for(int c = 0; c < 4; c++) {
        actualPoint = SOURCEPOINTS.at(c);
        point3D.at<double>(0, 0) = actualPoint.x;
        point3D.at<double>(0, 1) = actualPoint.y;

        tempPoint = secondMat * point3D;
        tempPoint = firstMat * tempPoint;

        actualPoint.x = tempPoint.at<double>(0, 0) / tempPoint.at<double>(2, 0);
        actualPoint.y = tempPoint.at<double>(1, 0) / tempPoint.at<double>(2, 0);

        destinationPoints.push_back(actualPoint);
    }

    //Create homography
    homography = findHomography(SOURCEPOINTS, destinationPoints, 0);

    //Transform the source image
    warpPerspective(SOURCEIMAGE, sourceWarped, homography, Size(512, 512));
    imshow("Camera Image", sourceWarped);
}

static void calcCameraPosition(int, void* userdata) {
    Vec<void*, 3>* data;
    Vec3i* spherCoord;
    Point3d* camPos;
    double pointOnXY;

    //Cast of the userdata
    data = static_cast<Vec<void*, 3>*>(userdata);
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
