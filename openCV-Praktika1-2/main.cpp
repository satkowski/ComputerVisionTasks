#include <main.h>

int main( int argc, const char** argv ) {
    Vec3i spherCoord (0,0,0);
    Vec3i affineTrans (0,0,0);
    Vec3i camPos;
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
    Vec3i* camPos;

    //Cast of the userdata
    data = static_cast<Vec<void*, 2> >(userdata);
    spherCoord = static_cast<Vec3i*>(data.val[0]);
    camPos = static_cast<Vec3i*>(data.val[1]);
}

static void calcAffineTransformation(int, void* userdata) {

}
