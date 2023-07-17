#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H
#include<QImage>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include<QPixmap>
#include<QBitmap>
/**
 * @brief The ImageUtil class util for opencv trans Qt image
 */
class ImageUtil
{
private:
     ImageUtil();
public:
    static QImage cvMat2QImage(cv::Mat &mat,float wh_ratio=0);

    static cv::Mat QImage_to_cvMat( const QImage &image, bool inCloneImageData = true ) ;

    static QPixmap cvMatToQPixmap(cv::Mat &inMat);

    static QBitmap loadIcon(QString path,int width,int height);
    //将指定颜色变成透明
    static cv::Mat toPng(cv::Mat &src,int mark);

};

#endif // IMAGEUTIL_H
