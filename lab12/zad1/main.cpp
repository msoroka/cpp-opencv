#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

bool motionDetector(Mat frame1, Mat frame2);

int main() {
    int k;
    int option = 1;
    VideoCapture cap(0);
    namedWindow("Motion", 1);
    Mat frame1, frame2;

    cap.read(frame2);

    char data_label_temp[50];
    double width = cap.get(CAP_PROP_FRAME_WIDTH);
    double height = cap.get(CAP_PROP_FRAME_HEIGHT);
    String fileName = "0.mov";
    bool pause = true;
    Size frame_size(width, height);
    int i = 1;

    VideoWriter outputVideo;

    chrono::steady_clock sc;
    auto start = sc.now();
    auto end = sc.now();
    auto time_span = static_cast<chrono::duration<double> >(end - start);

    time_t t = time(0);
    tm *now = localtime(&t);
    sprintf(data_label_temp, "%d%02d%02d%02d%02d%02d", (now->tm_year + 1900), (now->tm_mon + 1), (now->tm_mday),
            (now->tm_hour), (now->tm_min), (now->tm_sec));
    String label(data_label_temp);

    outputVideo.open(label + ".mov", -1, 30, frame_size, true);

    while (true) {
        frame1 = frame2.clone();
        if (pause) {
            cap.read(frame2);
        }

        Point textPoint = Point2i(30, frame2.rows - 30);
        Point textPoint2 = Point2i(30, 30);
        putText(frame2, label, textPoint, FONT_HERSHEY_PLAIN, 1, Scalar(255, 0, 0), 2);

        fileName = to_string(i) + ".mov";

        k = waitKey(1);
        if (k == 49 || k == 50 || k == 51)
            option = k;


        switch (option) {
            case 49:
                pause = true;
                if (motionDetector(frame1, frame2)) {
                    start = sc.now();
                }

                end = sc.now();
                time_span = static_cast<chrono::duration<double> >(end - start);

                if (time_span.count() < 2) {
                    outputVideo.write(frame2);
                    putText(frame2, "NAGRYWANIE", textPoint2, FONT_HERSHEY_PLAIN, 1, Scalar(255, 0, 0), 2);
                    putText(frame2, "RUCH", Point2i(30, 50), FONT_HERSHEY_PLAIN, 1, Scalar(255, 0, 0), 2);
                } else {
                    putText(frame2, "BRAK RUCHU", textPoint2, FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 2);
                }

                break;
            case 50:
                pause = false;
                putText(frame2, "PAUZA", textPoint2, FONT_HERSHEY_PLAIN, 1, Scalar(255, 0, 0), 2);

                break;
            case 51:
                putText(frame2, "NOWY PLIK", textPoint2, FONT_HERSHEY_PLAIN, 1, Scalar(255, 0, 0), 2);
                pause = true;
                i++;
                outputVideo.open(fileName, -1, 30, frame_size, true);

                option = 49;

                break;
        }


        imshow("Motion", frame2);
        if (waitKey(1) == 27)
            break;
    }
    cap.release();
    destroyAllWindows();
    return 0;
}


bool motionDetector(Mat frame1, Mat frame2) {
    Mat gray_frame1, gray_frame2;
    cvtColor(frame2, gray_frame2, COLOR_BGR2GRAY);
    cvtColor(frame1, gray_frame1, COLOR_BGR2GRAY);
    Mat diff;
    absdiff(gray_frame1, gray_frame2, diff);

    Mat blurr_diff;
    GaussianBlur(diff, blurr_diff, Size(5, 5), 20);

    Mat thresh;
    threshold(blurr_diff, thresh, 20.0, 255.0, THRESH_BINARY);

    vector<vector<Point> > contours;
    findContours(thresh, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    return contours.size() >= 20;
}
