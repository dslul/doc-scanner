#ifndef IMAGEWARP_H
#define IMAGEWARP_H

#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

class ImageWarp
{
public:
    ImageWarp();
    static bool warpScan(cv::Mat& origFrame, cv::Mat& output, std::vector<cv::Point2f> cornerPoints);
};

#endif // IMAGEWARP_H
