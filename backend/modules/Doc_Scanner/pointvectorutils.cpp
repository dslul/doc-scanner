#include "pointvectorutils.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

PointVectorUtils::PointVectorUtils()
{
}

/**
 * @brief scales a vector of points by a ratio ( 0 < ratio <=1 )
 * @param points
 * @param ratio
 */
void PointVectorUtils::scaleVector(std::vector<cv::Point2f>& points, double ratio)
{
    for (int var  = 0; var < points.size(); var++) {
        points[var] = cv::Point2f(points[var].x/ratio, points[var].y/ratio);
    }
}
/**
 * @brief Given a vector of four integer points, it returns the same points
 * converted to float and ordered clockwise, starting from top left corner
 * @param points
 * @return
 */
std::vector<cv::Point2f> PointVectorUtils::getOrdered4Points(std::vector<cv::Point> points)
{
    std::vector<cv::Point2f> orderedPoints(4);

    float max = points[0].x + points[0].y;
    orderedPoints[2] = points[0];
    float min = max;
    orderedPoints[0] = points[0];

    for (int var = 1; var < 4; var++) {
        if((points[var].x + points[var].y) > max) {
            max = points[var].x + points[var].y;
            //bottom right corner
            orderedPoints[2] = points[var];
        } else if((points[var].x + points[var].y) < min) {
            min = points[var].x + points[var].y;
            //top left corner
            orderedPoints[0] = points[var];
        }
    }

    max = points[0].y - points[0].x;
    orderedPoints[3] = points[0];
    min = max;
    orderedPoints[1] = points[0];

    for (int var = 1; var < 4; var++) {
        if((points[var].y - points[var].x) > max) {
            max = points[var].y - points[var].x;
            //bottom left corner
            orderedPoints[3] = points[var];
        } else if((points[var].y - points[var].x) < min) {
            min = points[var].y - points[var].x;
            //top right corner
            orderedPoints[1] = points[var];
        }
    }
    return orderedPoints;
}
