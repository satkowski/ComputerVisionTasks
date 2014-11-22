#include <main.h>

int main( int argc, const char** argv ) {
    Vec3i spherCoord (0,0,0);
    Vec3i affineTrans (0,0,0);
    Point3d camPos;
    Vec<void*, 2> data (&spherCoord, &camPos);

    //Creating the main window
    namedWindow("Image", 0);

    //Adding the trackbars for the spherical coordinates
    createTrackbar("radius", "Image", &(spherCoord.val[0]), 100, calcCameraPosition, &data);
    createTrackbar("theta ", "Image", &(spherCoord.val[1]), 100, calcCameraPosition, &data);
    createTrackbar("rho   ", "Image", &(spherCoord.val[2]), 100, calcCameraPosition, &data);

    //Adding the trackbars for the affine transformation
    createTrackbar("alpha", "Image", &(affineTrans.val[0]), 100, calcAffineTransformation, &affineTrans);
    createTrackbar("beta ", "Image", &(affineTrans.val[1]), 100, calcAffineTransformation, &affineTrans);
    createTrackbar("lamda", "Image", &(affineTrans.val[2]), 100, calcAffineTransformation, &affineTrans);

    waitKey();
    return 0;
}

static void calcCameraPosition(int, void* userdata) {
    Vec<void*, 2> data;
    Vec3i* spherCoord;
    Point3d* camPos;
    double pointOnXY;

    //Cast of the userdata
    data = static_cast<Vec<void*, 2> >(userdata);
    spherCoord = static_cast<Vec3i*>(data.val[0]);
    camPos = static_cast<Point3d*>(data.val[1]);

    //Calculate the lengt of the vector on the x-y-plane
    pointOnXY = spherCoord->val[1] * sin(double(spherCoord->val[2]));
    //Calculate the coordinates on the x and y axis with the vector
    camPos->x = pointOnXY * cos(double(spherCoord->val[3]));
    camPos->y = pointOnXY * sin(double(spherCoord->val[3]));
    //Calculate the coordinate on the z axis with theta
    camPos->z = spherCoord->val[1] * cos(double(spherCoord->val[2]));
}

static void calcAffineTransformation(int, void* userdata) {

}
