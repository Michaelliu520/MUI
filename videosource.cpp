#include "videosource.h"
#include<QDebug>

DeskCamera::DeskCamera(){
    // 任务执行完毕,该对象自动销毁
//    setAutoDelete(true);
}

void DeskCamera::init(){
    camera.open("ship.avi");
//    camera.open(0);
    if(!camera.isOpened()){
        qDebug()<<"打开摄像头失败";
        return ;
    }
    qDebug()<<"打开摄像头成功";
}

void DeskCamera::run(){
    while(true){
        camera >> frame;
//        camera.read(frame);
//        qDebug()<<"emit.....";
        if (frame.empty())
            {
                qDebug()<<"emit 失败";
                return;
            }
        emit sendFrame(frame);
    }
}

cv::Mat DeskCamera::getFrame(){
    return frame;
}

int DeskCamera::getFps(){
     fps = camera.get(cv::CAP_PROP_FPS);
     return fps;
}


DeskCamera::~DeskCamera(){
    camera.release();
}

//==========================HKCamera
HKCamera::HKCamera(){
    // 任务执行完毕,该对象自动销毁
    setAutoDelete(true);
}

void HKCamera::init(){
    camera1.open(0);
    if(!camera1.isOpened()){
        qDebug()<<"打开摄像头失败";
        return ;
    }
}

void HKCamera::run(){
    while(true){
        camera1>>frame;
    }
}

cv::Mat HKCamera::getFrame(){
    return frame;
}

int HKCamera::getFps(){
     fps = camera1.get(cv::CAP_PROP_FPS);
     return fps;
}


HKCamera::~HKCamera(){
    camera1.release();
}


