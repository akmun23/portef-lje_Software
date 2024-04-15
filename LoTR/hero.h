#ifndef HERO_H
#define HERO_H

#include <string>
class Hero
{
private:
    std::string _name;
    int _xp = 0;
    int _hp = 10;
    int _level = 1;
    int _strength = 2;

public:
    Hero(std::string name);
    void levelUp();
};

#endif // HERO_H
