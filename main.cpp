#include <QApplication>
#include "weapon.h"
int main(int argc, char *argv[])
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<cv::Mat>("const cv::Mat& ");

    QApplication a(argc, argv);
    Weapon weapon;
    weapon.Start();
    return a.exec();
}
