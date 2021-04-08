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
    // line l;
    // l.func();
    // Declare the output variables
    std::cout << 1 << '\n';
    Mat dst, cdst, cdstP;
    const char *default_file = "../flowchart1.jpg";
    const char *filename = argc >= 2 ? argv[1] : default_file;
    // Loads an image
    Mat src = imread(samples::findFile(filename), IMREAD_GRAYSCALE);
    // Check if image is loaded fine
    if (src.empty())
    {
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name -- default %s] \n", default_file);
        return -1;
    }
    // Edge detection
    Canny(src, dst, 50, 200, 3);
    // Copy edges to the images that will display the results in BGR
    cvtColor(dst, cdstP, COLOR_GRAY2BGR);
    
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
    // imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
    imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);
    // Wait and Exit
    waitKey();
    return 0;
}