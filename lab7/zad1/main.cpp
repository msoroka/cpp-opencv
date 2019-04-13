#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "track_color.h"

#include <iostream>

#define ESC 27

using namespace cv;
using namespace std;

int main() {
    VideoCapture stream(0);
    Mat image, imageHSV, imageTreshold;

    int selectedColor = 0;

    cout << "Wybierz kolor: " << endl;
    cout << "0. Żółty" << endl;
    cout << "1. Zielony" << endl;
    cout << "2. Niebieski" << endl << endl;
    cin >> selectedColor;

    if (selectedColor > 2) return 1;

    while (stream.read(image)) {
        flip(image, image, +1);

        cvtColor(image, imageHSV, COLOR_BGR2HSV);

        int high_h_arr[3] = {41, 82, 147};
        int high_s_arr[3] = {226, 253, 318};
        int high_v_arr[3] = {219, 154, 195};
        int low_h_arr[3] = {0, 34, 89};
        int low_s_arr[3] = {116, 65, 144};
        int low_v_arr[3] = {171, 75, 45};

        inRange(imageHSV,
                Scalar(low_h_arr[selectedColor], low_s_arr[selectedColor], low_v_arr[selectedColor]),
                Scalar(high_h_arr[selectedColor], high_s_arr[selectedColor], high_v_arr[selectedColor]),
                imageTreshold);

        erode(imageTreshold, imageTreshold, Mat(), Point(-1, -1), 2);
        dilate(imageTreshold, imageTreshold, Mat(), Point(-1, -1), 2);
        erode(imageTreshold, imageTreshold, Mat(), Point(-1, -1), 2);
        erode(imageTreshold, imageTreshold, Mat(), Point(-1, -1), 2);
        dilate(imageTreshold, imageTreshold, Mat(), Point(-1, -1), 2);
        dilate(imageTreshold, imageTreshold, Mat(), Point(-1, -1), 2);

        vector<vector<Point> > contours;

        findContours(imageTreshold, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
        drawContours(imageHSV, contours, 0, Scalar(100, 100, 100));

        vector<TrackColor> trackedColors;

        for (vector<Point> contour : contours) {
            TrackColor object(contour);

            object.setH((low_h_arr[selectedColor] + high_h_arr[selectedColor]) / 2);
            object.setS((low_s_arr[selectedColor] + high_s_arr[selectedColor]) / 2);
            object.setV((low_v_arr[selectedColor] + high_v_arr[selectedColor]) / 2);

            object.setCenter();

            trackedColors.push_back(object);
        }

        for (TrackColor object : trackedColors) {
            Point center = object.getCenter();

            circle(image, center, 20, (0, 0, 255));
            string str = to_string(center.x).append(", ").append(to_string(center.y));
            putText(image, str, center, FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255, 255));
        }

        imshow("Original", image);

        if (waitKey(30) == ESC) break;
    }

    return 0;
}
