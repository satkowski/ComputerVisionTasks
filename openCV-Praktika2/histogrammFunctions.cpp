#include <histogrammFunctions.h>

vector<Mat> histogrammEqualizationOneChannel(Mat *input) {
    /*------------- initialization ------------*/
    Mat tempInput;
    vector<Mat> output;
    Mat pdf, pdfImage;
    int histSize, histHeight, histWidth, binWidth;
    Mat cdf, cdfImage;
    float range[2] = { 0, 255 };
    const float* ranges[1];
    /*-----------------------------------------*/
    cvtColor(*input, tempInput, CV_BGR2GRAY);

    histSize = 256;
    *ranges = { range };

    histWidth = 512;
    histHeight = 400;
    binWidth = cvRound((double) histWidth/histSize);

    pdfImage = Mat(histHeight, histWidth, CV_8U, Scalar(0, 0, 0));
    cdfImage = Mat(histHeight, histWidth, CV_8U, Scalar(0, 0, 0));

    //Calculate histogram
    calcHist(&tempInput, 1, 0, Mat(), pdf, 1, &histSize, ranges, true, false);
    //Plot the pdf
    normalize(pdf, pdf, 0, pdfImage.rows, NORM_MINMAX, -1, Mat());
    //Draw every line between the points
    for(int c = 1; c < histSize; c++)
      line(pdfImage, Point(binWidth * (c - 1), histHeight - cvRound(pdf.at<float>(c - 1))),
                     Point(binWidth * c,       histHeight - cvRound(pdf.at<float>(c))),
                     Scalar(255, 0, 0),
                     2, 8, 0);

    cdf = Mat(pdf.rows, pdf.cols, CV_32F);
    cdf.at<float>(0) = pdf.at<float>(0);

    for(int cY = 1; cY < cdf.rows; cY++)
        cdf.at<float>(cY) = cdf.at<float>(cY - 1) + pdf.at<float>(cY);

    //Plot the cdf
    normalize(cdf, cdf, 0, cdfImage.rows, NORM_MINMAX, -1, Mat());
    //Draw every line between the points
    for(int c = 1; c < histSize; c++)
        line(cdfImage, Point(binWidth * (c - 1), histHeight - cvRound(cdf.at<float>(c - 1))),
                       Point(binWidth * c,       histHeight - cvRound(cdf.at<float>(c))),
                       Scalar(255, 0, 0),
                       2, 8, 0);

    output.push_back(pdfImage);
    output.push_back(cdfImage);

    return output;
}
