#ifndef ENEMY_H
#define ENEMY_H

#include <string>

class Enemy
{
private:
    std::string _name;
    int _hp;
    int _strength;
    int _xp;
public:
    Enemy(std::string name, int hp, int strength, int xp);
    std::string getName();
    int getHp();
    int getStrength();
};

#endif // ENEMY_H
