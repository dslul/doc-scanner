#ifndef SCANIMAGE_H
#define SCANIMAGE_H

#include <QObject>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "imagewarp.h"
#include "pointvectorutils.h"

class ScanImage : public QObject
{
    Q_OBJECT
    //Q_PROPERTY( QString helloWorld READ helloWorld WRITE setHelloWorld NOTIFY helloWorldChanged )

public:
    explicit ScanImage(QObject *parent = 0);
    ~ScanImage();
    Q_INVOKABLE QString elaborate(const QString &imgurl);
    Q_INVOKABLE void exportPdf(const QVariantList &imgurl);

protected:
    void extractScan(cv::Mat& frame, cv::Mat& output, bool moreRobust);
    //QString m_message;

};

#endif // SCANIMAGE_H
