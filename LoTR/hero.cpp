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
        _gold = query.value(6).toInt();
    }
}

Hero::Hero(std::string name):Character(name, 10, 2){
    query.prepare("INSERT INTO hero (name, hp, strength, level, xp, gold)"
                  "VALUES (:name, :hp, :strength, :level, :xp, :gold)");
    query.bindValue(":name", QString::fromStdString(name));
    query.bindValue(":hp", 10);
    query.bindValue(":strength", 2);
    query.bindValue(":level", 1);
    query.bindValue(":xp", 0);
    query.bindValue(":gold", 0);
    query.exec();
}

Hero::~Hero(){
    query.prepare("UPDATE hero "
                  "SET hp = :hp, strength = :strength, level = :level, xp = :xp, gold = :gold "
                  "WHERE name = :name");
    query.bindValue(":name", QString::fromStdString(getName()));
    query.bindValue(":hp", getHp());
    query.bindValue(":strength", getStrength());
    query.bindValue(":level", _level);
    query.bindValue(":xp", _xp);
    query.bindValue(":gold", _gold);
    query.exec();
}

void Hero::fight(Enemy enemy){
    int enemyHp = enemy.getHp();
    int enemyDamage = enemy.getStrength();
    std::cout << getName() << "(" << getHp() << " hp" << ")" << " vs. "
              << enemy.getName() << "(" << enemy.getHp() << " hp" << ")" << std::endl;

    std::cin.ignore(); // Clear input buffer

    while(1){
        std::cout << "Press enter to continue" << std::endl;
        std::cin.ignore();

        // Hero attacks enemy
        enemyHp -= getStrength();
        std::cout << getName() << " attacks " << enemy.getName() << " for " << getStrength() << " damage" << std::endl;
        std::cout << enemy.getName() << " has " << enemyHp << " hp" << std::endl;
        if(enemyHp <= 0){
            std::cout << "You won \n" << std::endl;
            query.exec("SELECT * FROM hero WHERE name = '" + QString::fromStdString(getName()) + "'");
            while(query.next()){
                setHp(query.value(2).toInt());
            }
            getXp(enemy.getXp());
            return;
        }

        // Enemy attacks hero
        setHp(getHp()-enemyDamage);
        std::cout << enemy.getName() << " attacks " << getName() << " for " << enemyDamage << " damage" << std::endl;
        std::cout << getName() << " has " << getHp() << " hp" << std::endl;

        if(getHp() <= 0){
            query.exec("SELECT * FROM hero WHERE name = '" + QString::fromStdString(getName()) + "'");
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

void Hero::getGold(int gold){
    _gold += gold;
}

void Hero::levelUp() {
    _xp -= _level*1000;
    _level++;
    setStrength(getStrength()+1);
    setHp(getHp()+2);
    std::cout << "You leveled up! You are now level " << _level << std::endl;
    // Update the values in the database
    query.prepare("UPDATE hero "
                  "SET level = :level, xp = :xp, strength = :strength, hp = :hp "
                  "WHERE name = :name");
    query.bindValue(":level", _level);
    query.bindValue(":xp", _xp);
    query.bindValue(":strength", getStrength());
    query.bindValue(":hp", getHp());
    query.bindValue(":name", QString::fromStdString(getName()));
    query.exec();
}
