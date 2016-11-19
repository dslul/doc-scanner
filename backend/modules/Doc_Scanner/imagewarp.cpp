#include "imagewarp.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

ImageWarp::ImageWarp()
{
}

bool ImageWarp::warpScan(cv::Mat& origFrame, cv::Mat& output, std::vector<cv::Point2f> cornerPoints)
{
    // width of the detected document
    double widthA = sqrt((pow((cornerPoints[2].x - cornerPoints[3].x),2)) + (pow((cornerPoints[2].y - cornerPoints[3].y),2)));
    double widthB = sqrt((pow((cornerPoints[1].x - cornerPoints[0].x),2)) + (pow((cornerPoints[1].y - cornerPoints[0].y),2)));
    double maxWidth = std::max(int(widthA), int(widthB));

    // height of the detected document
    double heightA = sqrt((pow((cornerPoints[1].x - cornerPoints[2].x),2)) + (pow((cornerPoints[1].y - cornerPoints[2].y),2)));
    double heightB = sqrt((pow((cornerPoints[0].x - cornerPoints[3].x),2)) + (pow((cornerPoints[0].y - cornerPoints[3].y),2)));
    double maxHeight = std::max(int(heightA), int(heightB));

    //the same order as in order4Points must be kept
    //top-left, top-right, bottom-right, bottom-left
    std::vector<cv::Point2f> warpPoints;
    warpPoints.push_back(cv::Point2f(0, 0));
    warpPoints.push_back(cv::Point2f(maxWidth-1, 0));
    warpPoints.push_back(cv::Point2f(maxWidth-1, maxHeight-1));
    warpPoints.push_back(cv::Point2f(0, maxHeight-1));

    cv::Mat transformationMatrix;
    transformationMatrix = cv::getPerspectiveTransform(cornerPoints, warpPoints);

    cv::warpPerspective(origFrame, output, transformationMatrix, cv::Size(maxWidth, maxHeight));

    return 0;
}
