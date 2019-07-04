#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <math.h>

#define CVUI_IMPLEMENTATION

#include "cvui.h"
#include "EnhancedWindow.h"

using namespace cv;
using namespace std;

#define ESC 27

CascadeClassifier faceCascade;
CascadeClassifier eyesCascade;
VideoCapture cap;
Mat frame(430, 640, CV_8UC3),
        frameGray,
        frameStaticFace,
        interface(Size(1366, 900), CV_8UC3);

bool enablePlay = false;
bool enableHideFace = false;
bool enableHideEyes = false;
bool enableSwapFaces = false;
bool enableVideoSave = false;
bool enabledVideoSaving = false;
bool enableVideoFramesSave = false;
bool enableSaveFaceFrame = false;
bool enableStaticSwap = false;
bool getFaces = false;
bool getStaticFace = false;
bool enableFaceSwapFromFile = false;
int framesCounter = 10;
int counterVideoFrames = 0;
int counterFaceFrames = 0;
int selectedFace = 0;

clock_t start;

vector<Rect> faces;
vector<Rect> facesTemp;

int main(int argc, char **argv) {
    interface = cv::Scalar(49, 52, 49);

    cvui::init("Window");

    faceCascade.load("/Users/msoroka/Desktop/uczelnia/obrazy/haarcascade_frontalface_alt.xml");
    eyesCascade.load("/Users/msoroka/Desktop/uczelnia/obrazy/haarcascade_eye_tree_eyeglasses.xml");

    VideoWriter outputVideo;
    char data_label_temp[50];

    while (true) {
        cvui::text(interface, frame.cols + 50, 20, "Odtwarzaj");
        cvui::checkbox(interface, frame.cols + 50, 40, "", &enablePlay);

        cvui::text(interface, frame.cols + 150, 20, "Zakrycie twarzy");
        cvui::checkbox(interface, frame.cols + 150, 40, "", &enableHideFace);

        cvui::text(interface, frame.cols + 270, 20, "Zakrycie oczu");
        cvui::checkbox(interface, frame.cols + 270, 40, "", &enableHideEyes);

        cvui::text(interface, frame.cols + 370, 20, "Zamiana twarzy");
        cvui::checkbox(interface, frame.cols + 370, 40, "", &enableSwapFaces);

        cvui::text(interface, frame.cols + 50, 180, "Zapis do wideo");
        cvui::checkbox(interface, frame.cols + 50, 200, "", &enableVideoSave);

        cvui::text(interface, frame.cols + 50, 230, "Ilosc klatek do zapisu");
        cvui::counter(interface, frame.cols + 50, 250, &framesCounter);

        cvui::text(interface, frame.cols + 230, 230, "Zapis klatek");
        cvui::checkbox(interface, frame.cols + 230, 250, "", &enableVideoFramesSave);

        cvui::text(interface, frame.cols + 350, 230, "Zapis twarzy do klatek");
        cvui::checkbox(interface, frame.cols + 350, 250, "", &enableSaveFaceFrame);

        cvui::text(interface, frame.cols + 50, 280, "Zamiana klatki");
        cvui::checkbox(interface, frame.cols + 50, 300, "", &enableStaticSwap);

        cvui::text(interface, frame.cols + 50, 320, "Zamiana twarzy z pliku");
        cvui::checkbox(interface, frame.cols + 50, 340, "", &enableFaceSwapFromFile);


        if (cvui::button(interface, frame.cols + 50, 80, "Obraz z kamerki")) {
            std::cout << "Obraz z kamery" << std::endl;
            cap.release();
            cap.open(0);
            cap.set(CAP_PROP_FRAME_WIDTH, 640);
            cap.set(CAP_PROP_FRAME_HEIGHT, 360);
        }

        if (cvui::button(interface, frame.cols + 50, 120, "Obraz z pliku wideo")) {
            cap.open("/Users/msoroka/Desktop/uczelnia/obrazy/projekt/video.mp4");
            cap.set(CAP_PROP_FRAME_WIDTH, 640);
            cap.set(CAP_PROP_FRAME_HEIGHT, 360);
        }

        if (enablePlay && !cap.isOpened()) {
            cap.release();
            cap.open(0);
            cap.set(CAP_PROP_FRAME_WIDTH, 640);
            cap.set(CAP_PROP_FRAME_HEIGHT, 360);
        }

        if (enablePlay) {
            cap >> frame;

            start = clock();

            cvtColor(frame, frameGray, COLOR_BGR2GRAY);
            equalizeHist(frameGray, frameGray);

            faceCascade.detectMultiScale(frameGray, faces, 1.1, 3, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
            Rect firstFaceArea;
            Mat firstFace;

            for (size_t i = 0; i < faces.size(); i++) {
                Rect faceArea = faces[i];

                Mat faceROI = frameGray(faces[i]);
                std::vector<Rect> eyes;

                eyesCascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

                if (!getFaces) {
                    facesTemp = faces;
                }

                getFaces = true;

                Point leftSide(faceArea.x, faceArea.y + faceArea.height / 2);
                Rect eyesArea(leftSide - Point(0, faceArea.height / 3), Size(faceArea.width, faceArea.height / 3));

                if (enableHideFace) {
                    GaussianBlur(frame(faceArea), frame(faceArea), Size(15, 15), 20);
                }

                if (enableHideEyes) {
                    rectangle(frame, eyesArea, Scalar(0, 0, 0), -1);
                }

                if (i == selectedFace) {
                    firstFaceArea = faceArea;
                    firstFace = frame(firstFaceArea).clone();

                    if (!getStaticFace) {
                        frameStaticFace = firstFace.clone();
                    }
                    getStaticFace = true;
                }
            }

            int width = 40;
            for (size_t i = 0; i < faces.size(); i++) {
                Rect face = faces[i];
                Mat img = frame.clone();
                Mat faceROI = img(face);
                Mat dst;
                resize(faceROI, dst, Size(100, 100), 0, 0, INTER_CUBIC);

                if (cvui::button(interface, width, frame.rows + 200, dst, dst, dst)) {
                    cout << i << endl;
                    selectedFace = i;
                    getStaticFace = false;
                }

                width += 150;

                if (enableSwapFaces) {
                    if (i != selectedFace) {
                        if (enableStaticSwap) {
                            resize(frameStaticFace, firstFace, faces[i].size());
                            firstFace.copyTo(frame(faces[i]));
                        } else {
                            resize(firstFace, firstFace, faces[i].size());
                            firstFace.copyTo(frame(faces[i]));
                        }
                    }
                }

                if(enableFaceSwapFromFile) {
                    Mat fileFace = imread("/Users/msoroka/Desktop/uczelnia/obrazy/projekt/frame_0_0.jpg");
                    Mat fileFaceGray;

                    cvtColor(fileFace, fileFaceGray, COLOR_BGR2GRAY);
                    equalizeHist(fileFaceGray, fileFaceGray);

                    faceCascade.detectMultiScale(fileFaceGray, facesTemp, 1.1, 3, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

                    Rect faceFileArea = facesTemp[0];
                    firstFace = fileFace(faceFileArea).clone();

                    for (auto & face : faces) {
                        resize(firstFace, firstFace, face.size());
                        firstFace.copyTo(frame(face));
                    }
                }
            }

            if (enableVideoSave) {
                if (enabledVideoSaving) {
                    outputVideo.write(frame);
                } else {
                    outputVideo.release();
                    time_t t = time(0);
                    tm *now = localtime(&t);
                    sprintf(data_label_temp, "%d%02d%02d%02d%02d%02d", (now->tm_year + 1900), (now->tm_mon + 1),
                            (now->tm_mday),
                            (now->tm_hour), (now->tm_min), (now->tm_sec));
                    String label(data_label_temp);
                    Size frame_size(frame.cols, frame.rows);
                    outputVideo.open(label + ".mov", -1, 30, frame_size, true);
                }

                enabledVideoSaving = true;
            } else {
                enabledVideoSaving = false;
            }

            if (enableVideoFramesSave) {
                std::cout << "Zapis klatki: " << counterVideoFrames << std::endl;

                if (counterVideoFrames < framesCounter && !frame.empty()) {
                    string name = "frame_" + to_string(counterVideoFrames) + ".jpg";
                    imwrite("/Users/msoroka/Desktop/uczelnia/obrazy/projekt/frames/" + name, frame);
                    counterVideoFrames++;
                } else {
                    counterVideoFrames = 0;
                    enableVideoFramesSave = false;
                }
            } else {
                counterVideoFrames = 0;
            }

            if (enableSaveFaceFrame) {
                std::cout << "Zapis klatki: " << counterFaceFrames << std::endl;

                if (counterFaceFrames < framesCounter && !frame.empty()) {
                    for (size_t i = 0; i < faces.size(); i++) {
                        string name = "frame_" + to_string(i) + "_" + to_string(counterFaceFrames) + ".jpg";
                        Mat img = frame.clone();
                        Rect face = faces[i];
                        Mat faceROI = img(face);
                        imwrite("/Users/msoroka/Desktop/uczelnia/obrazy/projekt/framesFace/" + name, faceROI);
                    }
                    counterFaceFrames++;
                } else {
                    counterFaceFrames = 0;
                    enableSaveFaceFrame = false;
                }
            } else {
                counterFaceFrames = 0;
            }
        }

        try {
            cvui::image(interface, 0, 0, frame);
            cvui::update();
            cvui::imshow("Window", interface);
        } catch (Exception &e) {
            cout << e.what() << endl;

            return 1;
        }

        if (waitKey(30) == ESC) break;
    }

    frame.release();
    frameGray.release();
    cap.release();

    return 0;
}