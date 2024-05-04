#include "hero.h"

Hero::Hero(int heroID, std::string name, int hp, int strength):Character(name, hp, strength){
    query.prepare("SELECT * "
                  "FROM hero "
                  "WHERE hero_id = :heroID");
    query.bindValue(":heroID", heroID);
    query.exec();
    while(query.next()){
        _level = query.value(4).toInt();
        _xp = query.value(5).toInt();
    }
}

Hero::Hero(std::string name):Character(name, 10, 2){
    query.prepare("INSERT INTO hero (name, hp, strength, level, xp)"
                  "VALUES (:name, :hp, :strength, :level, :xp)");
    query.bindValue(":name", QString::fromStdString(name));
    query.bindValue(":hp", 10);
    query.bindValue(":strength", 2);
    query.bindValue(":level", 1);
    query.bindValue(":xp", 0);
    query.exec();
}

Hero::~Hero(){
    query.prepare("UPDATE hero "
                  "SET hp = :hp, strength = :strength, level = :level, xp = :xp "
                  "WHERE name = :name");
    query.bindValue(":name", QString::fromStdString(getName()));
    query.bindValue(":hp", getHp());
    query.bindValue(":strength", getStrength());
    query.bindValue(":level", _level);
    query.bindValue(":xp", _xp);
    query.exec();
}

void Hero::fight(Enemy enemy){
    int enemyHp = enemy.getHp();
    int enemyDamage = enemy.getStrength();
    std::cin.ignore();
    while(1){
        std::cout << "Press enter to continue";
        std::cin.ignore();
        // Hero attacks enemy
        enemyHp -= getStrength();
        std::cout << getName() << " attacks " << enemy.getName() << " for " << getStrength() << " damage" << std::endl;
        std::cout << enemy.getName() << " has " << enemyHp << " hp" << std::endl;
        if(enemyHp <= 0){
            std::cout << "You won \n" << std::endl;
            getXp(enemy.getXp());
            query.exec("SELECT * FROM hero WHERE name = '" + QString::fromStdString(getName()) + "'");
            while(query.next()){
                setHp(query.value(2).toInt());
            }
            return;
        }

        // Enemy attacks hero
        setHp(getHp()-enemyDamage);
        std::cout << enemy.getName() << " attacks " << getName() << " for " << enemyDamage << " damage" << std::endl;
        std::cout << getName() << " has " << getHp() << " hp" << std::endl;

        if(getHp() <= 0){
            std::cout << "You died" << std::endl;
            return;
        }
    }
}

void Hero::getXp(int xp){
    _xp += xp;
    if(_xp >= _level*1000){
        levelUp();
    }
}

void Hero::levelUp() {
    _xp -= _level*1000;
    _level++;
    setStrength(getStrength()+1);
    setHp(getHp()+2);
}
