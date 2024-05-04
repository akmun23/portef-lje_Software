#include "enemy.h"

Enemy::Enemy(std::string name, int hp, int strength, int xp, std::string element):Character(name, hp, strength), _xp(xp), _element(element) {}

int Enemy::getXp(){
    return _xp;
}

