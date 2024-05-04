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
        _magic = query.value(7).toInt();
    }
}

Hero::Hero(std::string name):Character(name, 10, 2){
    query.prepare("INSERT INTO hero (name, hp, strength, level, xp, gold, magic)"
                  "VALUES (:name, :hp, :strength, :level, :xp, :gold, :magic)");
    query.bindValue(":name", QString::fromStdString(name));
    query.bindValue(":hp", 10);
    query.bindValue(":strength", 2);
    query.bindValue(":level", 1);
    query.bindValue(":xp", 0);
    query.bindValue(":gold", 0);
    query.bindValue(":magic", 0);
    query.exec();
}

Hero::~Hero(){
    query.prepare("UPDATE hero "
                  "SET hp = :hp, strength = :strength, level = :level, xp = :xp, gold = :gold, magic = :magic "
                  "WHERE name = :name");
    query.bindValue(":name", QString::fromStdString(getName()));
    query.bindValue(":hp", getHp());
    query.bindValue(":strength", getStrength());
    query.bindValue(":level", _level);
    query.bindValue(":xp", _xp);
    query.bindValue(":gold", _gold);
    query.bindValue(":magic", _magic);
    query.exec();
}

void Hero::fight(Enemy enemy){
    int enemyHp = enemy.getHp();
    int enemyDamage = enemy.getStrength();
    int magicDamage;
    int selfDamage;
    int magicCost;
    int magicType;
    bool magicUsed = false;

    std::cout << getName() << "(" << getHp() << " hp" << ")" << " vs. "
              << enemy.getName() << "(" << enemy.getHp() << " hp" << ")" << std::endl;

    std::cin.ignore(); // Clear input buffer

    while(1){
        int damage = getStrength();
        query.prepare("SELECT * FROM inventory "
                      "WHERE hero_id = (SELECT hero_id FROM hero WHERE name = :name)");
        query.bindValue(":name", QString::fromStdString(getName()));
        query.exec();
        if(query.next()){
            std::cout << "Do you want to use a magic item?" << std::endl;
            std::cout << "Press (0) to use item" << std::endl;
            std::cout << "Press (1) to continue" << std::endl;
            std::cout << "Enter choice: ";
            int choice;
            std::cin >> choice;
            std::cout << std::endl;
            if(choice == 0){
                std::cout << "You have " << getMagic() << " magic" << std::endl;
                while(1){
                    QSqlQuery magicQuery;
                    std::vector<int> magicItems;
                    std::cout << "Choose a magic item or press (0) to attack without an item" << std::endl;
                    // Find all the magic items the hero has
                    magicQuery.prepare("SELECT * FROM inventory "
                                       "WHERE hero_id = (SELECT hero_id FROM hero WHERE name = :name)");
                    magicQuery.bindValue(":name", QString::fromStdString(getName()));
                    magicQuery.exec();
                    while (magicQuery.next()) {
                        int magicID = magicQuery.value(2).toInt(); // Assuming magic_id is in the second column
                        query.prepare("SELECT name, magic_cost FROM magic WHERE magic_id = :magicID");
                        query.bindValue(":magicID", magicID);
                        query.exec();
                        if (query.next()) {
                            std::cout << "(" << magicID << ") " << query.value(0).toString().toStdString() << std::endl;
                            std::cout << "This item costs " << query.value(1).toInt() << " magic" << std::endl;
                            magicItems.push_back(magicID);
                        }
                    }
                    int magicID;
                    bool found = false;
                    std::cout << "Enter choice: ";
                    std::cin >> magicID;
                    if(magicID == 0){
                        break;
                    }
                    for(int i = 0; i < magicItems.size(); i++){
                        if(magicID == magicItems[i]){
                            found = true;
                            break;
                        } else if(i == magicItems.size()-1){
                            std::cout << "Invalid ID" << std::endl;
                        }
                    }
                    if(found){
                        query.prepare("SELECT * FROM magic WHERE magic_id = :magicID");
                        query.bindValue(":magicID", magicID);
                        query.exec();
                        query.next();
                        if(query.value(4).toInt() > getMagic()){
                            std::cout << "You do not have enough magic to use this item" << std::endl;
                            continue;
                        }
                        if(query.value(3) > getHp()){
                            std::cout << "You do not have enough hp to use this item" << std::endl;
                            continue;
                        }
                        magicDamage = query.value(2).toInt();
                        selfDamage = query.value(3).toInt();
                        magicCost = query.value(4).toInt();
                        magicType = query.value(5).toInt();
                        magicUsed = true;
                        break;
                    }
                }

            }
        } else {
            std::cout << "Press enter to continue" << std::endl;
            std::cin.ignore();
        }

        if(magicUsed){
            // Find enemy weakness
            query.prepare("SELECT element, weakness "
                          "FROM element "
                          "WHERE element_id = "
                          "(SELECT element_id "
                          "FROM enemy "
                          "WHERE name = :name)");
            query.bindValue(":name", QString::fromStdString(enemy.getName()));
            query.exec();
            query.next();
            std::string enemyType = query.value(0).toString().toStdString();
            std::string enemyWeakness = query.value(1).toString().toStdString();

            // Find magic type
            query.prepare("SELECT element, weakness "
                          "FROM element "
                          "WHERE element_id = :magicType");
            query.bindValue(":magicType", magicType);
            query.exec();
            query.next();
            std::string magicType = query.value(0).toString().toStdString();
            std::string magicWeakness = query.value(1).toString().toStdString();

            if(enemyType == magicWeakness){
                std::cout << "Your attack is weak against this enemy" << std::endl;
                magicDamage /= 2;
            } else if (enemyWeakness == magicType){
                std::cout << "Your attack is strong against this enemy" << std::endl;
                magicDamage *= 2;
            }

            std::string weakness = query.value(0).toString().toStdString();

            damage = magicDamage;
            setHp(getHp()-selfDamage);
            _magic -= magicCost;
            query.bindValue(":name", QString::fromStdString(getName()));
            query.exec();
        }

        // Hero attacks enemy
        enemyHp -= damage;
        std::cout << getName() << " attacks " << enemy.getName() << " for " << damage << " damage" << std::endl;
        std::cout << enemy.getName() << " has " << enemyHp << " hp" << std::endl;
        if(enemyHp <= 0){
            std::cout << "You won \n" << std::endl;
            query.exec("SELECT * FROM hero WHERE name = '" + QString::fromStdString(getName()) + "'");
            while(query.next()){
                setHp(query.value(2).toInt());
            }
            _magic = (_level*2)-2;
            gainXp(enemy.getXp());
            return;
        }

        // Enemy attacks hero
        setHp(getHp()-enemyDamage);
        std::cout << enemy.getName() << " attacks " << getName() << " for " << enemyDamage << " damage" << std::endl;
        std::cout << getName() << " has " << getHp() << " hp" << std::endl;

        if(getHp() <= 0){
            _magic = (_level*2)-2;
            query.exec("SELECT * FROM hero WHERE name = '" + QString::fromStdString(getName()) + "'");
            while(query.next()){
                setHp(query.value(2).toInt());
            }
            std::cout << "You died" << std::endl;
            return;
        }
    }
}

void Hero::gainXp(int xp){
    _xp += xp;
    if(_xp >= _level*1000){
        levelUp();
    }
}

void Hero::gainGold(int gold){
    _gold += gold;
    query.prepare("UPDATE hero "
                  "SET gold = :gold "
                  "WHERE name = :name");
    query.bindValue(":gold", _gold);
    query.bindValue(":name", QString::fromStdString(getName()));
    query.exec();
}

void Hero::levelUp() {
    _xp -= _level*1000;
    _level++;
    _magic += 2;
    setStrength(getStrength()+1);
    setHp(getHp()+2);
    std::cout << "You leveled up! You are now level " << _level << std::endl;
    // Update the values in the database
    query.prepare("UPDATE hero "
                  "SET level = :level, xp = :xp, strength = :strength, hp = :hp, magic = :magic "
                  "WHERE name = :name");
    query.bindValue(":level", _level);
    query.bindValue(":xp", _xp);
    query.bindValue(":strength", getStrength());
    query.bindValue(":hp", getHp());
    query.bindValue(":magic", _magic);
    query.bindValue(":name", QString::fromStdString(getName()));
    query.exec();
}

int Hero::getGold(){
    return _gold;
}

int Hero::getMagic(){
    return _magic;
}
