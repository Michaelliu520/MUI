#ifndef SEGMENTATION_H
#define SEGMENTATION_H
#include "net.h" // NCNN的头文件

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QDebug>
#include<QObject>
#include<QPoint>
#include<QRunnable>
class Segmentation:public QObject,public QRunnable
{
    Q_OBJECT
public:
    Segmentation();
    ~Segmentation();
    void init();
//    ncnn::Mat pre_handle_pic(cv::Mat &image);
    cv::Mat segmentation_mobilenetv2(cv::Mat& image);
//    cv::Point getPoint();

    void run() override;

private:

    cv::Mat _frame;
private:
    ncnn::Mat pre_handle_pic(cv::Mat &image);
    cv::Mat last_handle_pic(ncnn::Mat& image);

public slots:
      void receiveFrame(const cv::Mat& frame);

};

#endif // SEGMENTATION_H
