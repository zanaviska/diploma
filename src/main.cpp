#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    cv::Mat image;
    Mat dst, cdst, cdstP;
    const char *filename = "../flowchart1.jpg";
    // Loads an image
    Mat src = imread(samples::findFile(filename), IMREAD_GRAYSCALE);
    // Check if image is loaded fine
    if (src.empty())
    {
        printf(" Error opening image\n");
        return -1;
    }
    // Edge detection
    Canny(src, dst, 50, 200, 3);

    // Probabilistic Line Transform
    vector<Vec4i> linesP;                                 // will hold the results of the detection
    HoughLinesP(dst, linesP, 1, CV_PI / 180, 50, 50, 10); // runs the actual detection

    // Draw the lines
    for (size_t i = 0; i < linesP.size(); i++)
    {
        Vec4i l = linesP[i];
        line(cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
    }
    // Show results
    imshow("Source", src);
    imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);

    waitKey();
    return 0;
}