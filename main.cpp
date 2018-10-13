#include "gui.h"
#include "videoframe.h"
#include <QApplication>
#include <QThread>
#include <QTimer>
#include <QEventLoop>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <QDebug>
using namespace cv;
using namespace std;

// This is the wait time in milliseconds required to update frames (otherwise reads blank frames)
const int CAMERA_UPDATE_INTERVAL = 35;

int main(int argc, char** argv)
{
    QApplication gui(argc, argv);

    VideoFrame *feed = new VideoFrame(0);
    Menu *menu = new Menu();

    // Populates the menu with buttons
    menu->addButton("Gaussian Blur", feed, &VideoFrame::gaussianBlur);
    menu->addButton("Edge Detection", feed, &VideoFrame::detectEdges);
    menu->addButton("Gradient", feed, &VideoFrame::gradient);
    menu->addButton("Brightness", feed, &VideoFrame::adjustBrightness);
    menu->addButton("Contrast", feed, &VideoFrame::adjustContrast);
    menu->addButton("Negative", feed, &VideoFrame::negative);
    menu->addButton("Grayscale", feed, &VideoFrame::grayscale);
    menu->addButton("Resize (half)", feed, &VideoFrame::resizeToHalf);
    menu->addButton("Rotate 90º", feed, &VideoFrame::rotate90Degrees);
    menu->addButton("Horizontal Mirror", feed, &VideoFrame::horizontalMirror);
    menu->addButton("Vertical Mirror", feed, &VideoFrame::verticalMirror);
    menu->display();

    // Calls feed.updateFeed() repeatedly according to the defined wait time
    QTimer *timer = new QTimer(feed);
    QObject::connect(timer, &QTimer::timeout, feed, &VideoFrame::updateFeed);
    timer->start(CAMERA_UPDATE_INTERVAL);

    gui.exec();
    feed->~VideoFrame();
    return 0;
}
