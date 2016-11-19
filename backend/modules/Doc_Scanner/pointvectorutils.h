#ifndef POINTVECTORUTILS_H
#define POINTVECTORUTILS_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

class PointVectorUtils
{
public:
    PointVectorUtils();
    static void scaleVector(std::vector<cv::Point2f>& points, double ratio);
    static std::vector<cv::Point2f> getOrdered4Points(std::vector<cv::Point> points);
};

#endif // POINTVECTORUTILS_H
