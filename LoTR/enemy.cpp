#include "enemy.h"

Enemy::Enemy(std::string name, int hp, int strength, int xp):_name(name), _hp(hp), _strength(strength), _xp(xp) {

}

std::string Enemy::getName(){
    return _name;
}

int Enemy::getHp(){
    return _hp;
}

int Enemy::getStrength(){
    return _strength;
}

