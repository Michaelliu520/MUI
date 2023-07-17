#include "imageutil.h"

ImageUtil::ImageUtil()
{

}
QImage ImageUtil::cvMat2QImage(cv::Mat &mat,float wh_ratio)
{
    if(wh_ratio!=0){
        cv::Rect roi((mat.cols-mat.rows*wh_ratio)/2.0,0,mat.rows*wh_ratio,mat.rows);
        mat=mat(roi);
    }
    switch ( mat.type() )
    {
       // 8-bit, 4 channel
       case CV_8UC4:
       {
          QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32 );
          return image;
       }

       // 8-bit, 3 channel
       case CV_8UC3:
       {
          QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888 );
          return image.rgbSwapped();
       }

       // 8-bit, 1 channel
       case CV_8UC1:
       {
          static QVector<QRgb>  sColorTable;
          // only create our color table once
          if ( sColorTable.isEmpty() )
          {
             for ( int i = 0; i < 256; ++i )
                sColorTable.push_back( qRgb( i, i, i ) );
          }
          QImage image( mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8 );
          image.setColorTable( sColorTable );
          return image;
       }
       default:
          break;
    }
    return QImage();
}

cv::Mat ImageUtil::QImage_to_cvMat( const QImage &image, bool inCloneImageData) {
  switch ( image.format() )
  {
     // 8-bit, 4 channel
     case QImage::Format_RGB32:
     {
        cv::Mat mat( image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine() );
        return (inCloneImageData ? mat.clone() : mat);
     }

     // 8-bit, 3 channel
     case QImage::Format_RGB888:
     {
        if ( !inCloneImageData ) {
           std::cout << "ASM::QImageToCvMat() - Conversion requires cloning since we use a temporary QImage";
        }
        QImage swapped = image.rgbSwapped();
        return cv::Mat( swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine() ).clone();
     }

     // 8-bit, 1 channel
     case QImage::Format_Indexed8:
     {
        cv::Mat  mat( image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine() );

        return (inCloneImageData ? mat.clone() : mat);
     }

     default:
        qDebug("Image format is not supported: depth=%d and %d format\n", image.depth(), image.format());
        break;
  }

  return cv::Mat();
}

QPixmap ImageUtil::cvMatToQPixmap(cv::Mat &inMat)
{
    return QPixmap::fromImage(cvMat2QImage(inMat));
}
QBitmap ImageUtil::loadIcon(QString path,int width,int height){
    QPixmap pixmap(path);
    return pixmap.scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation).mask();
}
cv::Mat ImageUtil::toPng(cv::Mat &src,int mark)
{
    cv::Mat dst=src.clone();
    if (src.channels() != 4)
    {
        cv::cvtColor(src, dst, cv::COLOR_BGR2BGRA);
    }
    else
    {
        return dst;
    }
    for (int y = 0; y < dst.rows; ++y)
    {
        for (int x = 0; x < dst.cols; ++x)
        {
            cv::Vec4b & pixel = dst.at<cv::Vec4b>(y, x);
            if (pixel[0] == mark && pixel[1] == mark && pixel[2] == mark)
            {
                pixel[3] = 0;
            }
        }
    }
    return dst;
}
