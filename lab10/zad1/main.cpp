#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

#define ESC 27
#define Q 113
#define W 119
#define E 101

int main() {
    VideoCapture stream(0);
    Mat frame, frameGray;
    CascadeClassifier faceCascade;
    CascadeClassifier eyesCascade;
    vector<Rect> faces;

    faceCascade.load("/Users/msoroka/Desktop/uczelnia/obrazy/lab10/haarcascade_frontalface_alt.xml");
    eyesCascade.load("/Users/msoroka/Desktop/uczelnia/obrazy/lab10/haarcascade_eye_tree_eyeglasses.xml");

    namedWindow("Settings", 1);

    int scaleFactor = 3;
    int minNeighbors = 3;
    int minSize = 10;

    int key;

    createTrackbar("scaleFactor", "Settings", &scaleFactor, 100);
    createTrackbar("minNeighbors", "Settings", &minNeighbors, 100);
    createTrackbar("minSize", "Settings", &minSize, 100);

    while (true) {
        stream >> frame;

        cvtColor(frame, frameGray, COLOR_BGR2GRAY);
        equalizeHist(frameGray, frameGray);

        faceCascade.detectMultiScale(frameGray, faces, 1.1, 3, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
        Rect firstFaceArea;
        Mat firstFace;

        for (size_t i = 0; i < faces.size(); i++) {
            Rect faceArea = faces[i];

            Mat faceROI = frameGray(faces[i]);
            std::vector<Rect> eyes;

            //-- In each face, detect eyes
            eyesCascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

            Point leftSide(faceArea.x, faceArea.y + faceArea.height / 2);
            Rect eyesArea(leftSide - Point(0, faceArea.height / 3), Size(faceArea.width, faceArea.height / 3));

            // Gauss
            if (key == Q) {
                cout << "Gaussian blur" << endl;
                GaussianBlur(frame(faceArea), frame(faceArea), Size(15, 15), 20);
            }

            // Eyes
            if (key == W) {
                cout << "Hide eyes" << endl;
                rectangle(frame, eyesArea, Scalar(0, 0, 0), -1);
            }

            // Swap
            if (key == E) {
                cout << "Swap faces" << endl;
                if (i == 0) {
                    firstFaceArea = faceArea;
                    firstFace = frame(firstFaceArea).clone();
                } else {
                    resize(firstFace, firstFace, faceArea.size());
                    firstFace.copyTo(frame(faceArea));
                }
            }
        }

        key = waitKey(1);

        try {
            imshow("Camera", frame);
        } catch (Exception &e) {
            cout << e.what() << endl;

            return 1;
        }

        if (key == ESC) break;

        frame.release();
        frameGray.release();
        firstFace.release();
    }

    stream.release();

    return 0;
}