#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <iostream>

#define ESC 27

using namespace cv;
using namespace std;

int main() {
    VideoCapture stream(0);
    Mat original, foreground, background, edges, img_lines;
    Ptr<BackgroundSubtractorMOG2> pMOG2 = createBackgroundSubtractorMOG2();

    namedWindow("Background", 1);
    namedWindow("Motion", 1);
    namedWindow("Settings", 1);

    int line_length = 50;
    int line_gap = 10;
    int threshold = 50;

    createTrackbar("Line length", "Settings", &line_length, 100);
    createTrackbar("Line gap", "Settings", &line_gap, 100);
    createTrackbar("Threshold", "Settings", &threshold, 100);

    while (stream.read(original)) {
        Canny(original, edges, 50, 200, 3);

        vector<Vec4i> lines;
        HoughLinesP(edges, lines, 1, CV_PI / 180, threshold, line_length, line_gap);

        for (int i = 0; i < lines.size(); i++) {
            Vec4i l = lines[i];
            line(original, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
        }

        imshow("Motion", original);

        if (waitKey(30) == ESC) break;
    }

    destroyAllWindows();
    original.release();

    return 0;
}