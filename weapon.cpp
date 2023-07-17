#include "weapon.h"
#include <QThreadPool>
Weapon::Weapon()
{
    QThreadPool::globalInstance()->setMaxThreadCount(4);
}

void Weapon::Start(){
    w.show();
}
