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

    // compute the width of the new image, which will be the
    // maximum distance between bottom-right and bottom-left
    // x-coordiates or the top-right and top-left x-coordinates
    double widthA = sqrt((pow((cornerPoints[2].x - cornerPoints[3].x),2)) + (pow((cornerPoints[2].y - cornerPoints[3].y),2)));
    double widthB = sqrt((pow((cornerPoints[1].x - cornerPoints[0].x),2)) + (pow((cornerPoints[1].y - cornerPoints[0].y),2)));
    double maxWidth = max(int(widthA), int(widthB));
    cout << widthA << "   " << widthB << endl;
    // compute the height of the new image, which will be the
    // maximum distance between the top-right and bottom-right
    // y-coordinates or the top-left and bottom-left y-coordinates
    double heightA = sqrt((pow((cornerPoints[1].x - cornerPoints[2].x),2)) + (pow((cornerPoints[1].y - cornerPoints[2].y),2)));
    double heightB = sqrt((pow((cornerPoints[0].x - cornerPoints[3].x),2)) + (pow((cornerPoints[0].y - cornerPoints[3].y),2)));
    double maxHeight = max(int(heightA), int(heightB));
    cout << heightA << "   " << heightB << endl;

    vector<Point2f> warpPoints;
    warpPoints.push_back(Point2f(0, 0));
    warpPoints.push_back(Point2f(maxWidth-1, 0));
    warpPoints.push_back(Point2f(maxWidth-1, maxHeight-1));
    warpPoints.push_back(Point2f(0, maxHeight-1));

    //output = origFrame.clone();

    Mat transformationMatrix;
    transformationMatrix = getPerspectiveTransform(cornerPoints, warpPoints);

    warpPerspective(origFrame, output, transformationMatrix, Size(maxWidth, maxHeight));

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
        if((points[var].x + points[var].y) > max) {
            max = points[var].x + points[var].y;
            //bottom right
            orderedPoints[2] = points[var];
        } else if((points[var].x + points[var].y) < min) {
            min = points[var].x + points[var].y;
            //top left
            orderedPoints[0] = points[var];
        }
    }

    max = points[0].y - points[0].x;
    orderedPoints[3] = points[0];
    min = max;
    orderedPoints[1] = points[0];

    for (int var = 0; var < 4; var++) {
        if((points[var].y - points[var].x) > max) {
            max = points[var].y - points[var].x;
            //bottom left
            orderedPoints[3] = points[var];
        } else if((points[var].y - points[var].x) < min) {
            min = points[var].y - points[var].x;
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
