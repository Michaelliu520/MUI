#ifndef DETECTION_H
#define DETECTION_H
#include "layer.h"
#include "net.h"


#if defined(USE_NCNN_SIMPLEOCV)
#include "simpleocv.h"
#else
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#endif

#include <float.h>
#include <stdio.h>
#include <vector>
#include <QRunnable>
#include <QObject>
#include <QDebug>
#include <QTimer>
#define YOLOV5_V60 1 //YOLOv5 v6.0
//#define YOLOV5_V62 1 //YOLOv5 v6.2 export  onnx model method https://github.com/shaoshengsong/yolov5_62_export_ncnn

#if YOLOV5_V60 || YOLOV5_V62
#define MAX_STRIDE 64
#else
#define MAX_STRIDE 32
class YoloV5Focus : public ncnn::Layer
{
public:
    YoloV5Focus()
    {
        one_blob_only = true;
    }

    virtual int forward(const ncnn::Mat& bottom_blob, ncnn::Mat& top_blob, const ncnn::Option& opt) const
    {
        int w = bottom_blob.w;
        int h = bottom_blob.h;
        int channels = bottom_blob.c;

        int outw = w / 2;
        int outh = h / 2;
        int outc = channels * 4;

        top_blob.create(outw, outh, outc, 4u, 1, opt.blob_allocator);
        if (top_blob.empty())
            return -100;

#pragma omp parallel for num_threads(opt.num_threads)
        for (int p = 0; p < outc; p++)
        {
            const float* ptr = bottom_blob.channel(p % channels).row((p / channels) % 2) + ((p / channels) / 2);
            float* outptr = top_blob.channel(p);

            for (int i = 0; i < outh; i++)
            {
                for (int j = 0; j < outw; j++)
                {
                    *outptr = *ptr;

                    outptr += 1;
                    ptr += 2;
                }

                ptr += w;
            }
        }

        return 0;
    }
};

DEFINE_LAYER_CREATOR(YoloV5Focus)
#endif //YOLOV5_V60    YOLOV5_V62

struct Object
{
    cv::Rect_<float> rect;
    int label;
    float prob;
};

class Detection:public QObject,public QRunnable
{
    Q_OBJECT
public:
    Detection();
    ~Detection();
    void run() override;
    void draw_objects(const cv::Mat& bgr, const std::vector<Object>& objects);
public:
    std::vector<Object> objects;
    cv::Mat _frame;
private:
    int detect_yolov5(const cv::Mat& bgr, std::vector<Object>& objects);
    void qsort_descent_inplace(std::vector<Object>& faceobjects, int left, int right);
    void qsort_descent_inplace(std::vector<Object>& faceobjects);
    void nms_sorted_bboxes(const std::vector<Object>& faceobjects, std::vector<int>& picked, float nms_threshold, bool agnostic = false);
    void generate_proposals(const ncnn::Mat& anchors, int stride, const ncnn::Mat& in_pad, const ncnn::Mat& feat_blob, float prob_threshold, std::vector<Object>& objects);

private:
    QTimer timer;
    bool isStart;

public slots:
    void receiveFrame(const cv::Mat& frame);
};


inline float intersection_area(const Object& a, const Object& b)
{
    cv::Rect_<float> inter = a.rect & b.rect;
    return inter.area();
}
inline float sigmoid(float x)
{
    return static_cast<float>(1.f / (1.f + exp(-x)));
}

#endif // DETECTION_H
