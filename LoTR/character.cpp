#include "character.h"

Character::Character(std::string name, int hp, int strength):_name(name), _hp(hp), _strength(strength) {}

void Character::createHero(){
    query.prepare("INSERT INTO hero (name, hp, strength, level, xp)"
                  "VALUES (:name, :hp, :strength, :level, :xp)");
    query.bindValue(":name", QString::fromStdString(_name));
    query.bindValue(":hp", 10);
    query.bindValue(":strength", 2);
    query.bindValue(":level", 1);
    query.bindValue(":xp", 0);
    query.exec();
}

std::string Character::getName(){
    return _name;
}

void Character::setName(std::string name){
    _name = name;
}

int Character::getHp(){
    return _hp;
}

void Character::setHp(int hp){
    _hp = hp;
}

int Character::getStrength(){
    return _strength;
}

void Character::setStrength(int strength){
    _strength = strength;
}
