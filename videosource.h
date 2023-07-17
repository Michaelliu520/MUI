#ifndef VIDEOSOURCE_H
#define VIDEOSOURCE_H
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include<QRunnable>
#include<QObject>
//简单工厂模式
class VideoSource:public QObject,public QRunnable
{
    Q_OBJECT
public:
    double fps;
    cv::Mat frame;
public:
    virtual void init() = 0;
    virtual cv::Mat getFrame() = 0;
    virtual int getFps()=0;
    virtual ~VideoSource() {};
signals:
    void sendFrame(const cv::Mat& frame);
};

class DeskCamera:public VideoSource
{
private:
    cv::VideoCapture camera;
public:
    explicit DeskCamera();
    void init() override;
    int getFps() override;
    cv::Mat getFrame() override;
    void run() override;
    ~DeskCamera();
};

class HKCamera:public VideoSource
{
public:
    cv::VideoCapture camera1;
    explicit HKCamera();
    void init() override;
    int getFps() override;
    cv::Mat getFrame() override;
    void run() override;
    ~HKCamera();
};


class AbstractFactory{
public:
    virtual VideoSource* createCamera() = 0;
    virtual ~AbstractFactory(){};
};

class DeskCameraFactory:public AbstractFactory
{
    VideoSource* createCamera() override{
        return new DeskCamera;
    }
    ~DeskCameraFactory(){}
};
class HKCameraFactory:public AbstractFactory{
    VideoSource* createCamera() override{
        return new HKCamera;
    }
    ~HKCameraFactory(){}
};


//策略模式
enum class Level:int {experiment_room,experiment_lake};

class SelectCamera{
private:
    AbstractFactory* camera_factory;
//    VideoSource* camera_source;
public:
    VideoSource* select(Level level){
        switch (level) {
            case Level::experiment_room:{
                camera_factory = new DeskCameraFactory;
                return camera_factory->createCamera();
                break;
            }

            case Level::experiment_lake:{
                camera_factory = new HKCameraFactory;
                return camera_factory->createCamera();
                break;
            }
            default:{

                return nullptr;
                break;
            }
        }
    }
};

#endif // VIDEOSOURCE_H
