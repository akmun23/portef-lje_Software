#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"
#include <string>

class Enemy : public Character
{
private:
    int _xp;

public:
    Enemy(std::string name, int hp, int strength, int xp);
    int getXp();
};

#endif // ENEMY_H
