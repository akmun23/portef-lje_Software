#include "hero.h"

Hero::Hero(int heroID) {
    query.prepare("SELECT * "
                  "FROM hero "
                  "WHERE hero_id = :heroID");
    query.bindValue(":heroID", heroID);
    query.exec();
    while(query.next()){
        _name = query.value(1).toString().toStdString();
        _hp = query.value(2).toInt();
        _strength = query.value(3).toInt();
        _level = query.value(4).toInt();
        _xp = query.value(5).toInt();
    }
}

Hero::Hero(std::string name):_name(name){
    query.prepare("INSERT INTO hero (name, hp, strength, level, xp)"
                  "VALUES (:name, :hp, :strength, :level, :xp)");
    query.bindValue(":name", QString::fromStdString(_name));
    query.bindValue(":hp", _hp);
    query.bindValue(":strength", _strength);
    query.bindValue(":level", _level);
    query.bindValue(":xp", _xp);
    query.exec();
}

Hero::~Hero(){
    query.prepare("UPDATE hero "
                  "SET hp = :hp, strength = :strength, level = :level, xp = :xp "
                  "WHERE name = :name");
    query.bindValue(":name", QString::fromStdString(_name));
    query.bindValue(":hp", _hp);
    query.bindValue(":strength", _strength);
    query.bindValue(":level", _level);
    query.bindValue(":xp", _xp);
    query.exec();
}

void Hero::giveXp(int xp){
    _xp += xp;
    if(_xp >= _level*1000){
        levelUp();
    }
}

void Hero::levelUp() {
    _level++;
    _strength++;
    _hp += 2;
    _xp -= 1000;
}

std::string Hero::getName(){
    return _name;
}

int Hero::getHp(){
    return _hp;
}

int Hero::getStrength(){
    return _strength;
}
