#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

#define ESC 27
#define O 111
#define P 112
#define T 116

void draw(vector<vector<Point> > contours, Mat frame2);

int main() {
    VideoCapture cap(0);
    namedWindow("Motion", 1);
    Mat frame1, frame2, gray_frame1, gray_frame2, diff, blurr_diff, thresh, foreground, gauss;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    int option, optionChoosen;
    bool flag = true;
    bool toogle = true;

    cap.read(frame2);
    while (true) {
        frame1 = frame2.clone();

        if (toogle) {
            cap.read(frame2);
        }

        // Konwersja klatek do GRAY
        cvtColor(frame2, gray_frame2, COLOR_BGR2GRAY);
        cvtColor(frame1, gray_frame1, COLOR_BGR2GRAY);

        // Obliczenie różnicy
        absdiff(gray_frame1, gray_frame2, diff);

        // Filtr wygładzający Gaussa
        GaussianBlur(diff, blurr_diff, Size(5, 5), 20);

        // Binaryzacja
        threshold(blurr_diff, thresh, 20.0, 255.0, THRESH_BINARY);

        // Znalezienie konturu
        findContours(thresh, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
        gauss = frame2.clone();

        optionChoosen = waitKey(1);
        if (optionChoosen == P || optionChoosen == T || optionChoosen == O) {
            option = optionChoosen;
            if (optionChoosen == P) {
                toogle = !toogle;
            }
        }

        switch (option) {
            case P: {
                cout << "P" << endl;
                draw(contours, frame2);
                break;
            }
            case T: {
                cout << "T" << endl;
                draw(contours, frame2);
                break;
            }
            case O: {
                cout << "O" << endl;
                break;
            }
        }

        imshow("Motion", frame2);

        if (waitKey(1) == ESC) break;
    }

    return 0;
}


void draw(vector<vector<Point> > contours, Mat frame2) {
    if (!contours.empty()) {
        vector<Point> max_contour;
        int largest_area = -1;
        Mat hull;
        Rect bounding_rect;

        // Znalezienie największeko konturu
        for (int i = 0; i < contours.size(); i++) {
            double a = contourArea(contours[i]);
            if (a > largest_area) {
                largest_area = a;
                max_contour = contours[i];
            }
        }

        // Prostokąt poruszającego się obiektu
        bounding_rect = boundingRect(max_contour);

        // Znalezienie otoczki wypukłej
        convexHull(max_contour, hull);

        // Rysowanie
        vector<vector<Point> > drawing = {hull};
        rectangle(frame2, bounding_rect, Scalar(255, 255, 255));
        drawContours(frame2, drawing, -1, Scalar(255, 255, 255));

        Point pt;
        pt.x = max_contour.begin()->x;
        pt.y = max_contour.begin()->y;

        string str = to_string(pt.x).append(", ").append(to_string(pt.y));
        putText(frame2, str, pt, FONT_HERSHEY_PLAIN, 2, Scalar(255, 255, 255));

    }
}