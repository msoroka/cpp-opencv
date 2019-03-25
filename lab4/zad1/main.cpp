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
    // Zmienne pomocnicze
    VideoCapture stream;
    Mat image, firstFrame, secondFrame, thirdFrame, result;

    // Zmienne, w k†órych przechowywana jest wartość tych sliderów z opcjami
    int alphaSlider = 1;
    int thresholdSlider = 1;

    // Wybór do menu
    int choice = 0;

    cout << "1. Kamerka laptopa" << endl;
    cout << "2. Plik wideo bike" << endl;
    cout << "3. Plik wideo robot_no_loop" << endl;
    cout << "Podaj żródło obrazu:" << endl;
    cin >> choice;

    // Wybieramy źródło obrazu
    switch (choice) {
        case 1: {
            // Kamerka internetowa
            stream.open(0);
            break;
        }
        case 2: {
            // Wideo z dziewczynką na rowerze
            stream.open("/Users/msoroka/Desktop/uczelnia/obrazy/lab4/bike.avi");
            break;
        }
        case 3: {
            // Wideo z jeżdżącym robotem
            stream.open("/Users/msoroka/Desktop/uczelnia/obrazy/lab4/robot_no_loop.avi");
            break;
        }
        default: {
            cout << "Błędny wybór." << endl;
            return -1;
        }
    }

    // Sprawdzenie czy poprawnie załadowano źródło obrazu
    if (!stream.isOpened()) {
        cout << "Błąd podczas otwierania kamery." << endl;
    } else {
        cout << "Pomyślnie otworzono źródło obrazu." << endl;
    }

    // Złapanie pierwszej klatki obrazu, do dalszych porównań
    stream >> firstFrame;

    // Zrobienie kopii pierwszej klatki 
    secondFrame = firstFrame;

    // Nałożenie szarej barwy na zapisane klatki
    cvtColor(firstFrame, firstFrame, COLOR_BGR2GRAY);
    cvtColor(secondFrame, secondFrame, COLOR_BGR2GRAY);

    image = firstFrame;

    namedWindow("Camera", WINDOW_AUTOSIZE);
    namedWindow("Settings", WINDOW_AUTOSIZE);

    // Inicjalizacja sliderków z opcjami
    createTrackbar("Alpha", "Settings", &alphaSlider, 300);
    createTrackbar("Threshold", "Settings", &thresholdSlider, 300);

    while (true) {
        // Wczytanie klatki z aktualnym obrazem
        stream >> secondFrame;
        stream >> thirdFrame;

        // Sprawdzenie czy klatki nie są puste, żeby nie wysypało błędu
        if (secondFrame.empty() || thirdFrame.empty()) {

            // Ustawienie obrazu z powrotem na pierwszą klatkę, aby zapętlić
            // odtwarzanie obrazu
            // !! Nie wpływa na obraz z kamerki wideo
            stream.set(CAP_PROP_POS_FRAMES, 0);

            // Ponowne wczytanie klatek
            stream >> secondFrame;
            stream >> thirdFrame;
        }

        // Nałożnie barwy szarości
        cvtColor(secondFrame, secondFrame, COLOR_BGR2GRAY);
        cvtColor(thirdFrame, thirdFrame, COLOR_BGR2GRAY);

        // Obliczenie różnicy pomiędzy wczytanymi klatkami
        absdiff(secondFrame, thirdFrame, result);
        threshold(result, result, thresholdSlider, 255, THRESH_BINARY);

        // Obliczenia współczynnika alfa z wzoru podanego na slajdach
        // i zastosowanie go na obrazie
        double alpha = (double) alphaSlider / 100;
        image = (1 - alpha) * image + alpha * result;

        try {
            imshow("Camera", image);
        } catch (Exception &e) {
            cout << e.what() << endl;

            return 1;
        }

        if (waitKey(30) == ESC) break;
    }

    stream.release();
    image.release();
    firstFrame.release();
    secondFrame.release();
    thirdFrame.release();
    result.release();
    destroyAllWindows();

    return 0;
}