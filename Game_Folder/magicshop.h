#ifndef MAGICSHOP_H
#define MAGICSHOP_H

#include "hero.h"

class MagicShop
{
public:
    QSqlQuery query;
    MagicShop();
    bool enterShop(Hero &hero);
    void getInfo();
    void showWares();
    void showInventory(Hero &hero);
    void buyItem(Hero &hero);
};

#endif // MAGICSHOP_H
