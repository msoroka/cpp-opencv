#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <time.h>


#define ESC 27

using namespace std;
using namespace cv;
using namespace chrono;

int main() {
    VideoCapture stream(0);
    Mat image, imageHSV, imageThreshold;

    int low_h_arr[3] = {0, 34, 89}, high_h_arr[3] = {41, 82, 147},
            low_s_arr[3] = {116, 65, 144}, high_s_arr[3] = {226, 253, 318},
            low_v_arr[3] = {171, 75, 45}, high_v_arr[3] = {219, 154, 195};
    int low_h = 0, high_h = 41, low_s = 116, high_s = 226, low_v = 171, high_v = 219;
    int pos_x, pos_y;
    vector<vector<Point>> contours;

    stream.set(CAP_PROP_FRAME_WIDTH, 640);
    stream.set(CAP_PROP_FRAME_HEIGHT, 480);

    namedWindow("Settings", WINDOW_AUTOSIZE);

    int i = 0;
    int temp = 0;

    long t;

    while (stream.read(image)) {
        t = time(0);

        if (t % 5 == 0) {
            if (temp == 0) {

                cout << "Zmiana" << endl;

                low_h = low_h_arr[i];
                high_h = high_h_arr[i];
                low_s = low_s_arr[i];
                high_s = high_s_arr[i];
                low_v = low_v_arr[i];
                high_v = high_v_arr[i];

                i += 1;
                temp += 1;

                if (i > 2) {
                    i = 0;
                }
            }
        }else {
            temp = 0;
        }

        flip(image, image, +1);

        cvtColor(image, imageHSV, COLOR_BGR2HSV);


        inRange(imageHSV, Scalar(low_h, low_s, low_v), Scalar(high_h, high_s, high_v), imageThreshold);

        erode(imageThreshold, imageThreshold, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        erode(imageThreshold, imageThreshold, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        dilate(imageThreshold, imageThreshold, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
        dilate(imageThreshold, imageThreshold, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

        findContours(imageThreshold, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
        drawContours(imageThreshold, contours, 0, Scalar(100, 100, 100));

        Moments oMoments = moments(imageThreshold);

        double dM01 = oMoments.m01;
        double dM10 = oMoments.m10;
        double area = oMoments.m00;

        pos_x = static_cast<int>(dM10 / area);
        pos_y = static_cast<int>(dM01 / area);

        if (pos_x >= 0 && pos_y >= 0) {
            circle(image, Point(pos_x, pos_y), 20, (0, 0, 255));
            line(image, Point(pos_x, pos_y), Point(pos_x, pos_y), Scalar(0, 0, 255), 1);
        }

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