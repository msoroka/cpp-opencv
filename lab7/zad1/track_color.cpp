#include "track_color.h"

void TrackColor::setH(int value) {
    this->v = value;
}

void TrackColor::setS(int value) {
    this->s = value;
}

void TrackColor::setV(int value) {
    this->v = value;
}

TrackColor::TrackColor(vector<Point> contour) {
    this->contour = contour;
}

void TrackColor::setColor(string color) {
    this->color = color;
}

Point TrackColor::getCenter() {
    return this->center;
}

Point TrackColor::setCenter() {
    Moments oMoments = moments(this->contour);

    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double area = oMoments.m00;

    this->center = Point2f((float) dM10 / area, (float) dM01 / area);
}