#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>

#define ESC 27

using namespace std;
using namespace cv;

int main() {
    VideoCapture stream(0);
    Mat image, imageHSV, imageThreshold;

    int low_h, high_h, low_s, high_s, low_v, high_v;
    int pos_x, pos_y;
    vector<vector<Point>> contours;

    stream.set(CAP_PROP_FRAME_WIDTH, 640);
    stream.set(CAP_PROP_FRAME_HEIGHT, 480);

    namedWindow("Settings", WINDOW_AUTOSIZE);
    createTrackbar("Low H", "Settings", &low_h, 365);
    createTrackbar("High H", "Settings", &high_h, 365);
    createTrackbar("Low S", "Settings", &low_s, 365);
    createTrackbar("High S", "Settings", &high_s, 365);
    createTrackbar("Low V", "Settings", &low_v, 365);
    createTrackbar("High V", "Settings", &high_v, 365);

    while (stream.read(image)) {
        cout << "low_h: " << low_h << " high_h: " << high_h << endl;
        cout << "low_s: " << low_s << " high_s: " << high_s << endl;
        cout << "low_v: " << low_v << " high_v: " << high_v << endl;

        flip(image, image, +1);

        cvtColor(image, imageHSV, COLOR_BGR2HSV);

        inRange(imageHSV, Scalar(low_h, low_s, low_v), Scalar(high_h, high_s, high_v), imageThreshold);

        erode(imageThreshold, imageThreshold, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        erode(imageThreshold, imageThreshold, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        dilate(imageThreshold, imageThreshold, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        dilate(imageThreshold, imageThreshold, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

        findContours(imageThreshold, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
        drawContours(imageThreshold, contours, 0, Scalar(100,100,100));

        Moments oMoments = moments(contours[0]);

        double dM01 = oMoments.m01;
        double dM10 = oMoments.m10;
        double area = oMoments.m00;

        pos_x = static_cast<int>(dM10 / area);
        pos_y = static_cast<int>(dM01 / area);

        if (pos_x >= 0 && pos_y >= 0)
            circle(image, Point(pos_x, pos_y), 20, (0,0,255));
            line(image, Point(pos_x, pos_y), Point(pos_x, pos_y), Scalar(0,0,255), 1);

        try {
            imshow("Original", image);
            imshow("HSV", imageHSV);
            imshow("Thresholded", imageThreshold);
        } catch (Exception &e) {
            cout << e.what() << endl;

            return 1;
        }

        if (waitKey(30) == ESC) break;
    }

    stream.release();
    image.release();
    imageHSV.release();
    imageThreshold.release();
    destroyAllWindows();

    return 0;
}