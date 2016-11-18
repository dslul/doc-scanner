#ifndef SCANIMAGE_H
#define SCANIMAGE_H

#include <QObject>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/core/core.hpp>

class ScanImage : public QObject
{
    Q_OBJECT
    //Q_PROPERTY( QString helloWorld READ helloWorld WRITE setHelloWorld NOTIFY helloWorldChanged )

public:
    explicit ScanImage(QObject *parent = 0);
    ~ScanImage();
    Q_INVOKABLE QString elaborate(const QString &imgurl);

protected:

    //QString m_message;

//private:
//    bool warpScan(cv::Mat& origFrame, cv::Mat& output, std::vector<cv::Point2f> cornerPoints);
//    std::vector<cv::Point2f> getCornerPoints(const cv::Mat& img);
//    void nonMaxRegionSuppression(cv::Mat& segmented);
//    void extractScan(cv::Mat& frame, cv::Mat& output, bool moreRobust);

};

#endif // SCANIMAGE_H
