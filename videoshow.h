#ifndef VIDEOSHOW_H
#define VIDEOSHOW_H

#include <QWidget>
#include <QTimer>
#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <mutex>
#include <QPainter>
#include <QDebug>
#include "videosource.h"
#include "imageutil.h"
#include "detection.h"
#include "segmentation.h"
class VideoShow:public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
private:
    cv::Mat _frame;
    QImage currentImage;
    std::mutex mtx;
    QPainter painter;
private:
    VideoSource* player;
    AbstractFactory* player_factory;

    Detection* detector;
    Segmentation* segmenter;

    void init(Level level);
public:
    VideoShow(QWidget* parent = nullptr);

//    VideoShow() = delete;
    ~VideoShow();
protected:
    void initializeGL() override
        {
            initializeOpenGLFunctions();
        }

    void paintGL() override
        {
            glClear(GL_COLOR_BUFFER_BIT);
            std::lock_guard<std::mutex> lock(this->mtx);
//            detector->draw_objects(currentImage, detector->objects);
            if (!currentImage.isNull())
            {
                painter.begin(this);
                painter.drawImage(QRect(0,0,width(),height()),currentImage);
                painter.end();
            }
        }

public slots:
    void receiveFrame(const cv::Mat& frame);

};

#endif // VIDEOSHOW_H
