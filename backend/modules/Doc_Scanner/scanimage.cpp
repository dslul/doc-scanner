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

void smoothBinarization(Mat& output)
{

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

vector<Point2f> order4Points(vector<Point> points)
{
    vector<Point2f> orderedPoints(4);

    float max = points[0].x + points[0].y;
    orderedPoints[2] = points[0];
    float min = max;
    orderedPoints[0] = points[0];

    for (int var = 0; var < 4; var++) {
        if(points[var].x + points[var].y > max) {
            max = points[var].x + points[var].y;
            //bottom right
            orderedPoints[2] = points[var];
        } else if(points[var].x + points[var].y < min) {
            min = points[var].x + points[var].y;
            //top left
            orderedPoints[0] = points[var];
        }
    }

    max = points[0].x - points[0].y;
    orderedPoints[3] = points[0];
    min = max;
    orderedPoints[1] = points[0];

    for (int var = 0; var < 4; var++) {
        if(points[var].x - points[var].y > max) {
            max = points[var].x - points[var].y;
            //bottom left
            orderedPoints[3] = points[var];
        } else if(points[var].x - points[var].y < min) {
            min = points[var].x - points[var].y;
            //top right
            orderedPoints[1] = points[var];
        }
    }
    return orderedPoints;
}

void scaleVector(vector<Point2f>& points, double ratio) {
    for (int var  = 0; var < points.size(); var++) {
        points[var] = Point2f(points[var].x/ratio, points[var].y/ratio);
    }
}

void extractScan(Mat& frame, Mat& output, bool moreRobust)
{
    double ratio = 500.0 / frame.rows;
    Mat orig = frame.clone();
    resize(frame, frame, Size(0, 0), ratio, ratio);

    Mat gray;
    cvtColor(frame, gray, CV_BGR2GRAY);

    //moderate blur to eliminate noise and apply canny edge detection
    GaussianBlur(gray, gray, Size(5, 5), 0);
    Mat thres;
    double high_thres = threshold(gray, thres, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    double low_thres = high_thres * 0.5;
    cout << high_thres << endl;
    Mat cannyedged;
    Canny(gray, cannyedged, low_thres, high_thres);
    //Canny(gray, cannyedged, 75, 200);

    //find contours
    vector<vector<Point> > contours;
    findContours(cannyedged, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

    //keep only largest contours
    QMap<double, vector<Point> > map;

    for( int i = 0; i< contours.size(); i++ ) {
        map.insert(contourArea(contours.at(i)), contours.at(i));
    }
    //get only the 10 largest contours and search for the paper
    vector<Point> paperContourInt;
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
            paperContourInt = approx;
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

    vector<Point2f> paperContourf = order4Points(paperContourInt);
    scaleVector(paperContourf, ratio);

    warpScan(orig, output, paperContourf);

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
