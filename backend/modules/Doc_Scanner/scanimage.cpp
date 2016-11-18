#include "scanimage.h"
#include <QStandardPaths>
#include <QMap>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

ScanImage::ScanImage(QObject *parent) :
    QObject(parent)
{
}

ScanImage::~ScanImage() {
}

bool warpScan(Mat& origFrame, Mat& output, vector<Point2f> cornerPoints)
{

    //resize(origFrame, output, Size(297*5, 210*5));

    vector<Point2f> warpPoints;
    warpPoints.push_back(Point2f(0, 0));
    warpPoints.push_back(Point2f(0, origFrame.rows));
    warpPoints.push_back(Point2f(origFrame.cols, origFrame.rows));
    warpPoints.push_back(Point2f(origFrame.cols, 0));

    output = origFrame.clone();

    Mat transformationMatrix;
    transformationMatrix = getPerspectiveTransform(cornerPoints, warpPoints);

    warpPerspective(origFrame, output, transformationMatrix, output.size());

    return 0;
}


void extractScan(Mat& frame, Mat& output, bool moreRobust)
{
    double ratio = 500.0 / frame.rows;
    Mat orig = frame.clone();
    resize(frame, frame, Size(0, 0), ratio, ratio);

    Mat gray;
    cvtColor(frame, gray, CV_BGR2GRAY);

    GaussianBlur(gray, gray, Size(5, 5), 0);

    Mat edged;
    Canny(gray, edged, 75, 200);

    //find contours
    vector<vector<Point> > contours;
    findContours(edged, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    //keep only largest contours
    QMap<double, vector<Point> > map;

    for( int i = 0; i< contours.size(); i++ ) {
        map.insert(contourArea(contours.at(i)), contours.at(i));
    }
    //get only the 10 largest contours and search for the paper
    vector<Point> paperContour;
    QMap<double, vector<Point> >::const_iterator i = map.constEnd();
    i--;

    bool isBorderFound = false;
    for(int index=0; i != map.constBegin() && index < 10; i--, index++) {
        double peri = arcLength(i.value(), true);
        vector<Point> approx;
        approxPolyDP(i.value(), approx, 0.02*peri, true);
        if(approx.size() == 4) {
            cout << "FOUND BORDER" << endl;
            isBorderFound = true;
            paperContour = approx;
            break;
        }
    }

    if(!isBorderFound) {
        output = frame.clone();
        return;
    }

    //vector<vector<Point> > finContours;
    //finContours.push_back(paperContour);
    //drawContours(orig, finContours, -1, Scalar(0, 255, 0), 2);

    //output = orig.clone();

    vector<Point2f> sheetContour(4);
    sheetContour[0] = Point2f(paperContour[0].x/ratio, paperContour[0].y/ratio);
    sheetContour[1] = Point2f(paperContour[1].x/ratio, paperContour[1].y/ratio);
    sheetContour[2] = Point2f(paperContour[2].x/ratio, paperContour[2].y/ratio);
    sheetContour[3] = Point2f(paperContour[3].x/ratio, paperContour[3].y/ratio);

    warpScan(orig, output, sheetContour);

    cvtColor(output, output, COLOR_BGR2GRAY);
    adaptiveThreshold(output, output, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 251, 10);

    return;



}

QString ScanImage::elaborate(const QString &imgurl)
{
    QString imgfile = imgurl.right(imgurl.size()-7);
    Mat input = imread(imgfile.toStdString());
    cout << imgfile.toStdString() << endl;
    Mat scan;
    extractScan(input, scan, true);

    String destfile = (QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/" +
                       (imgfile.right(imgfile.size()-imgfile.lastIndexOf('/')-1) + "_out.png")).toStdString();
    cout << destfile << endl;
    imwrite(destfile, scan);
    return QString::fromStdString(destfile);
    std::cout << "working" << std::endl;
}
