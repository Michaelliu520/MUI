#ifndef WEAPON_H
#define WEAPON_H

#include "mainwindow.h"
#include "videosource.h"
#include "videoshow.h"

#include <QObject>
class Weapon
{
private:
    VideoSource* player;
    AbstractFactory* player_factory;
    MainWindow w;
public:
    Weapon();
    void Start();
};

#endif // WEAPON_H
