#include "scanimage.h"
#include "imagewarp.h"
#include "pointvectorutils.h"
#include <QStandardPaths>
#include <QMap>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <QPdfWriter>
#include <QPainter>
#include <QPainter>
#include <QVariant>

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


void ScanImage::extractScan(Mat& frame, Mat& output, bool moreRobust)
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

    for(int i = 0; i< contours.size(); i++) {
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

    vector<Point2f> paperContourf = PointVectorUtils::getOrdered4Points(paperContourInt);
    PointVectorUtils::scaleVector(paperContourf, ratio);

    ImageWarp::warpScan(orig, output, paperContourf);

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

void ScanImage::exportPdf(const QVariantList &imgurl)
{
    QPdfWriter pdfwriter(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/test.pdf");
    QPainter painter(&pdfwriter);
    pdfwriter.setPageSize(QPagedPaintDevice::A4);

    for (int var = 0; var < imgurl.size(); var++) {
        painter.drawPixmap(QRect(0,0,pdfwriter.logicalDpiX()*8.3,pdfwriter.logicalDpiY()*11.7),
                           QPixmap((imgurl.at(var)).toString()));
        //if it's not the last page add another one
        if(var != imgurl.size() - 1)
            pdfwriter.newPage();
    }

    painter.end();
    cout << "Image has been written to the pdf file!" << endl;
}
