#include <main.h>

int main( int argc, const char** argv ) {
    String sourceName;
    int xPos, xNeg, yPos, yNeg;

    Vec3i spherCoord (0, 0, 0);
    Point3d camPos;
    Vec3i affineTrans (0, 0, 1);
    Mat rotMatX = Mat_<double>(3, 3);
    Mat rotMat = Mat_<double>(3, 3);

    Vec<void*, 4> data (&spherCoord, &camPos, &rotMat, &affineTrans);

    const char* keyMap;
    //Standard image that will be used if dont exist arguments
    keyMap = "{1       |   |../Bilder/62962.jpg }";

    //Reading the Callingarguments
    CommandLineParser parser(argc, argv, keyMap);
    sourceName = parser.get<String>("1");

    //----- Calculating static variables ---------------
    //Creating the images and testing if it is empty or not
    SOURCE_IMAGE = imread(sourceName);
    if(SOURCE_IMAGE.empty()) {
        printf("Cannot read the image %s\n", sourceName.c_str());
        return -1;
    }    

    //Calculate the center of the image
    U0 = SOURCE_IMAGE.cols / 2;
    V0 = SOURCE_IMAGE.rows / 2;

    //Fill the instrinsic camera parameter matrix
    INTRINSIC_CAM_PAR_MAT = (Mat_<double>(3, 3) <<
                             ALPHA, 0,      U0,
                             0,     ALPHA,  V0,
                             0,     0,      1);

    //Add the corners of the source image
    SOURCE_POINTS_ORIG.push_back(Point2f(0,                     0));                         //Upper left corner
    SOURCE_POINTS_ORIG.push_back(Point2f(SOURCE_IMAGE.cols - 1, 0));                         //Upper right corner
    SOURCE_POINTS_ORIG.push_back(Point2f(0,                     SOURCE_IMAGE.rows - 1));     //Down left corner
    SOURCE_POINTS_ORIG.push_back(Point2f(SOURCE_IMAGE.cols - 1, SOURCE_IMAGE.rows - 1));     //Down right corner

    //Calculating the new coordinates for the center of the image
    xNeg = 0 - U0;
    xPos = SOURCE_IMAGE.cols - U0 - 1;
    if(SOURCE_IMAGE.rows % 2 == 1) {    yNeg = 0 - U0;      yPos = SOURCE_IMAGE.rows - V0 - 1;  }
    else {                              yNeg = 0 - V0 + 1;  yPos = SOURCE_IMAGE.rows - V0;      }

    //Add the corners of the shifted image on the plan in the 3d room
    SOURCE_POINTS_SHIFT.push_back(Point2f(xNeg, yNeg));      //Down left corner
    SOURCE_POINTS_SHIFT.push_back(Point2f(xPos, yNeg));      //Down right corner
    SOURCE_POINTS_SHIFT.push_back(Point2f(xNeg, yPos));      //Upper left corner
    SOURCE_POINTS_SHIFT.push_back(Point2f(xPos, yPos));      //Upper right corner
    SOURCE_POINTS_SHIFT.push_back(Point2f(0, 0));            //Upper right corner
    //-----------------------------------------------------

    //Filling the rotation matrix
    rotMat = (Mat_<double>(3, 3) <<
              cos(0),   -sin(0),    0,
              sin(0),   cos(0),     0,
              0,        0,          1);

//    //Filling the rotation matrix
//    rotMatX = (Mat_<double>(3, 3) <<
//              1,    0,           0,
//              0,    cos(0),     -sin(0),
//              0,    sin(0),      cos(0));
//    rotMatY = (Mat_<double>(3, 3) <<
//              cos(0),   0,  sin(0),
//              0,        1,  0,
//              -sin(0),  0,  cos(0));

    //Creating the original image window
    namedWindow("Original Image", 0);
    imshow("Original Image", SOURCE_IMAGE);

    //Creating the camera window
    namedWindow("Camera Image");

    //Adding the trackbars for the spherical coordinates
    createTrackbar("radius", "Camera Image", &(spherCoord.val[0]), ALPHA * 15, calcCameraImage, &data);
    createTrackbar("theta ", "Camera Image", &(spherCoord.val[1]), 90, calcCameraImage, &data);
    createTrackbar("rho   ", "Camera Image", &(spherCoord.val[2]), 360, calcRotationMatrix, &data);

    //Creating the affine transformation window
    namedWindow("Affine Transformation");

    //Adding the trackbars for the affine transformation
    createTrackbar("alpha", "Affine Transformation", &(affineTrans.val[0]), 360, calcAffineTransformation, &data);
    createTrackbar("beta ", "Affine Transformation", &(affineTrans.val[1]), 360, calcAffineTransformation, &data);
    createTrackbar("lamda", "Affine Transformation", &(affineTrans.val[2]), 15, calcAffineTransformation, &data);

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
    rotMat->at<double>(0, 0) = cos(dValue * PI / 180);
    rotMat->at<double>(1, 0) = sin(dValue * PI / 180);
    rotMat->at<double>(0, 1) = -rotMat->at<double>(1, 0);
    rotMat->at<double>(1, 1) = rotMat->at<double>(0, 0);

    //Calling the camera position method
    calcCameraImage(0, userdata);
}

static void calcCameraImage(int, void* userdata) {
    Vec<void*, 4>* data;
    Point3d* camPos;
    Mat* rotMat;

    Mat extrinsicCamMat = Mat_<double>(3, 4);
    Mat point3D = Mat_<double>(4, 1);

    Point2f actualPoint;
    Mat tempPoint = Mat_<double>(3, 1);
    vector<Point2f> destinationPoints;

    Point2f outputCenter, centerOffset;

    Mat homography, sourceWarped;

    calcCameraPosition(0, userdata);

    //Cast of the userdata
    data = static_cast<Vec<void*, 4>*>(userdata);
    camPos = static_cast<Point3d*>(data->val[1]);
    rotMat = static_cast<Mat*>(data->val[2]);

    //Fill the extrinsic camera parameter matrix
    for(int cY = 0; cY < 3; cY++) {
        for(int cX = 0; cX < 3; cX++)
            extrinsicCamMat.at<double>(cY, cX) = rotMat->at<double>(cY, cX);
    }
    extrinsicCamMat.at<double>(0, 3) = camPos->x;
    extrinsicCamMat.at<double>(1, 3) = camPos->y;
    extrinsicCamMat.at<double>(2, 3) = camPos->z;

    //Fill the point vector from the image
    point3D.at<double>(0, 2) = 0;
    point3D.at<double>(0, 3) = 1;

    for(int c = 0; c < SOURCE_POINTS_SHIFT.size(); c++) {
        actualPoint = SOURCE_POINTS_SHIFT.at(c);
        point3D.at<double>(0, 0) = actualPoint.x;
        point3D.at<double>(0, 1) = actualPoint.y;

        tempPoint = INTRINSIC_CAM_PAR_MAT * (extrinsicCamMat * point3D);

        actualPoint.x = tempPoint.at<double>(0, 0) / tempPoint.at<double>(0, 2);
        actualPoint.y = tempPoint.at<double>(0, 1) / tempPoint.at<double>(0, 2);

        if(c < SOURCE_POINTS_SHIFT.size() - 1) destinationPoints.push_back(actualPoint);
    }

    //Centering of the image
    outputCenter = actualPoint;
    centerOffset = outputCenter - Point2f(255, 255);
    for(int c = 0; c < SOURCE_POINTS_SHIFT.size() - 1; c++)
        destinationPoints.at(c) -= centerOffset;
    outputCenter -= centerOffset;

    //Create homography
    homography = findHomography(SOURCE_POINTS_ORIG, destinationPoints, 0);

    //Transform the source image
    warpPerspective(SOURCE_IMAGE, sourceWarped, homography, Size(512, 512));

    //Save the data
    destinationPoints.push_back(outputCenter);
    CAMERA_IMAGE_POINTS = destinationPoints;
    CAMERA_IMAGE = sourceWarped;

    //Show the image
    imshow("Camera Image", sourceWarped);

    calcAffineTransformation(0, userdata);
}

static void calcCameraPosition(int, void* userdata) {
    Vec<void*, 4>* data;
    Vec3i* spherCoord;
    Point3d* camPos;
    double lengthOnXY;

    //Cast of the userdata
    data = static_cast<Vec<void*, 4>*>(userdata);
    spherCoord = static_cast<Vec3i*>(data->val[0]);
    camPos = static_cast<Point3d*>(data->val[1]);

    //Calculate the lengt of the vector on the x-y-plane
    lengthOnXY = spherCoord->val[0] * sin(spherCoord->val[1] * PI / 180);
    //Calculate the coordinates on the x and y axis with the vector
    camPos->x = lengthOnXY * cos(spherCoord->val[2] * PI / 180);
    camPos->y = lengthOnXY * sin(spherCoord->val[2] * PI / 180);
    //Calculate the coordinate on the z axis with theta
    camPos->z = spherCoord->val[0] * cos(spherCoord->val[1] * PI / 180);
}

static void calcAffineTransformation(int, void* userdata) {
    Vec<void*, 4>* data;
    Vec3i* affineTrans;
    Mat scalingMat, rotMatAlpha, rotMatBeta, transformMat;
    Mat augmentedMat = Mat_<double>(2, 2);

    Mat point2D = Mat_<double>(3, 1);
    Point2f actualPoint;
    Mat tempPoint = Mat_<double>(3, 1);
    vector<Point2f> destinationPoints;

    Point2f outputCenter, centerOffset;

    Mat homography, sourceWarped;

    //Cast of the userdata
    data = static_cast<Vec<void*, 4>*>(userdata);
    affineTrans = static_cast<Vec3i*>(data->val[3]);

    //Filling the needed matriz
    scalingMat = (Mat_<double>(2, 2) <<
                            affineTrans->val[2], 0,
                            0,                   1);
    rotMatAlpha = (Mat_<double>(2, 2) <<
                            cos(affineTrans->val[0] * PI / 180), -sin(affineTrans->val[0] * PI / 180),
                            sin(affineTrans->val[0] * PI / 180), cos(affineTrans->val[0] * PI / 180));
    rotMatBeta = (Mat_<double>(2, 2) <<
                            cos(affineTrans->val[1] * PI / 180), -sin(affineTrans->val[1] * PI / 180),
                            sin(affineTrans->val[1] * PI / 180), cos(affineTrans->val[1] * PI / 180));

    //Calculation for the augmented matrix
    augmentedMat = rotMatAlpha * (-rotMatBeta * (scalingMat * rotMatBeta));

    //Fill the transormation matrix
    transformMat = (Mat_<double>(3, 3) <<
                            augmentedMat.at<double>(0, 0), augmentedMat.at<double>(0, 1), U0,
                            augmentedMat.at<double>(1, 0), augmentedMat.at<double>(1, 1), V0,
                            0                            , 0                            , 1);

    //Fill the point vector from the image
    point2D.at<double>(0, 2) = 1;

    for(int c = 0; c < CAMERA_IMAGE_POINTS.size(); c++) {
        actualPoint = CAMERA_IMAGE_POINTS.at(c);
        point2D.at<double>(0, 0) = actualPoint.x;
        point2D.at<double>(0, 1) = actualPoint.y;

        tempPoint = transformMat * point2D;

        actualPoint.x = tempPoint.at<double>(0, 0) / tempPoint.at<double>(0, 2);
        actualPoint.y = tempPoint.at<double>(0, 1) / tempPoint.at<double>(0, 2);

        if(c < CAMERA_IMAGE_POINTS.size() - 1)  destinationPoints.push_back(actualPoint);
    }

    //Centering of the image
    outputCenter = actualPoint;
    centerOffset = outputCenter - Point2f(255, 255);
    for(int c = 0; c < CAMERA_IMAGE_POINTS.size() - 1; c++)
        destinationPoints.at(c) -= centerOffset;

    //Create homography
    homography = findHomography(SOURCE_POINTS_ORIG, destinationPoints, 0);

    //Transform the source image
    warpPerspective(SOURCE_IMAGE, sourceWarped, homography, Size(512, 512));
    imshow("Affine Transformation", sourceWarped);
}
