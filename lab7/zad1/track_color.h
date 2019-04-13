#ifndef ZAD1_TRACK_COLOR_H
#define ZAD1_TRACK_COLOR_H

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>

using namespace cv;
using namespace std;

class TrackColor {
private:
    vector<Point> contour;
    Point center;
    int h, s, v;
    string color;

public:
    TrackColor(vector<Point> contour);

    void setColor(string color);

    Point getCenter();

    Point setCenter();

    void setH(int value);

    void setS(int value);

    void setV(int value);
};

#endif
