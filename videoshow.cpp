#include "videoshow.h"
#include <QApplication>
#include <QDesktopWidget>

#include <QThreadPool>

VideoShow::VideoShow(QWidget* parent):QOpenGLWidget(parent),
detector(new Detection()),
segmenter(new Segmentation())
{
    setFixedSize(QSize(960,540));
    player_factory = new DeskCameraFactory();
    player = player_factory->createCamera();
    player->init();

    connect(player,&VideoSource::sendFrame,this,&VideoShow::receiveFrame);
    connect(player,&VideoSource::sendFrame,detector,&Detection::receiveFrame);
    connect(player,&VideoSource::sendFrame,segmenter,&Segmentation::receiveFrame);

    QThreadPool::globalInstance()->start(player);
    QThreadPool::globalInstance()->start(detector);
    QThreadPool::globalInstance()->start(segmenter);

    qDebug()<<QThreadPool::globalInstance()->activeThreadCount();
}
VideoShow::~VideoShow(){
    delete  player;
    delete  player_factory;
    delete  detector;
}

void VideoShow::receiveFrame(const cv::Mat& frame){
//    cv::imshow("ll",frame);
//    cv::waitKey(1);
    std::lock_guard<std::mutex> lock(this->mtx);
    _frame = frame;
//    qDebug()<<"receive frame";
    currentImage = QImage(_frame.data, _frame.cols, _frame.rows, _frame.step, QImage::Format_BGR888);
//    currentImage = ImageUtil::cvMat2QImage(_frame);

    update();
}


void VideoShow::init(Level level){
//    camera = select_camera->select(level);
//    camera->start();
}

