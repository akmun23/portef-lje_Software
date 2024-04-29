#ifndef HERO_H
#define HERO_H

#include "character.h"
#include <string>

class Hero : public Character
{
private:
    int _xp;
    int _level;

public:
    Hero(int heroID);
    Hero(std::string name);
    ~Hero();
    void getXp(int xp);
    void levelUp();
};

#endif // HERO_H
