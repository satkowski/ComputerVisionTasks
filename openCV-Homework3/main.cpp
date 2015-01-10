#include <main.h>


int main( int argc, const char** argv ) {
    /*------------- initialization ------------*/
    String filename;
    Mat image, outputImage;
    /*-----------------------------------------*/

    const char* keyMap;
    //Standard image that will be used if dont exist arguments
    keyMap = "{path     |   |../Bilder/txt.png }";

    //Reading the Callingarguments
    CommandLineParser parser(argc, argv, keyMap);
    filename = parser.get<String>("path");

    //Creating the image and testing if it is empty or not
    image = imread(filename);
    if(image.empty()) {
        printf("Cannot read the image %s\n", filename.c_str());
        return -1;
    }
    image.copyTo(outputImage);

    //Creating windows for the images
    namedWindow("Original Image", 0);
    imshow("Original Image", image);
    namedWindow("Fourier Image", 0);
    imshow("Fourier Image", fourier(image));

    waitKey();
    return 0;
}

Mat fourier(Mat& input) {
    Mat tempInput;
    cvtColor(input, tempInput, CV_BGR2GRAY);

    Mat padded;                              //expand input image to optimal size
    int m = getOptimalDFTSize( tempInput.rows );
    int n = getOptimalDFTSize( tempInput.cols ); // on the border add zero values
    copyMakeBorder(tempInput, padded, 0, m - tempInput.rows, 0, n - tempInput.cols, BORDER_CONSTANT, Scalar::all(0));
    padded.convertTo(padded, CV_32F);

    Mat planes[] = {padded, Mat::zeros(padded.rows, padded.cols, CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(complexI, complexI);            // this way the result may fit in the source matrix

    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(input))^2 + Im(DFT(input))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(input), planes[1] = Im(DFT(input))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magI = planes[0];

    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
                                          // viewable image form (float between values 0 and 1).

    return magI;
}
