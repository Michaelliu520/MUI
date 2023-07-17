#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QThreadPool>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      videoShow(this)
{
    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect screenGeometry = desktopWidget->screenGeometry(0);
    this->setGeometry(screenGeometry);



    videoShow.show();

}

MainWindow::~MainWindow()
{
}

