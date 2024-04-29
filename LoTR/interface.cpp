#include "interface.h"

Interface::Interface() {
    query.exec("SELECT * FROM enemy");
    while(query.next()) {
        std::string name = query.value(1).toString().toStdString();
        int hp = query.value(2).toInt();
        int strength = query.value(3).toInt();
        int xp = query.value(4).toInt();
        enemies.push_back(Enemy(name, hp, strength, xp));
    }

    // Create heroes from a database containing the data
    query.exec("SELECT * FROM hero");
    while(query.next()) {
        int hero_id = query.value(0).toInt();
        heroes.push_back(Hero(hero_id));
    }

}

void Interface::heroSelection(){
    int selectLoop = 0;
    while(1){
        // Give choice of selecting hero or create new one
        if(selectLoop == 0) {
            std::cout << "Do you want to create a new Hero or select an existing one?" << std::endl;
            std::cout << "Press (0) to create a new hero" << std::endl;
            std::cout << "Press (1) to select an existing hero" << std::endl;
        }

        std::string input;
        std::cin >> input;

        if(input == "0") {
            std::string name;
            std::cout << "Enter name for hero: ";
            std::cin >> name;
            heroes.push_back(Hero(name));
            _currHero = heroes.size()-1;
            break;
        } else if(input == "1") {
            if(heroes.size() == 0) {
                std::cout << "No heroes available" << std::endl;
                continue;
            } else {
                std::cout << "Heroes available: ";
                for(int i = 0; i < heroes.size(); i++) {
                    std::cout << heroes[i].getName();
                    if(i != heroes.size()-1) {
                        std::cout << ", ";
                    }
                }
                std::cout << std::endl;
            }
            std::string name;
            std::cout << "Enter name for hero: ";
            std::cin >> name;
            bool found = false;
            for(int i = 0; i < heroes.size(); i++) {
                std::string heroName = heroes[i].getName();
                if(heroName == name){
                    _currHero = i;
                    found = true;
                }
            }
            if(found) {
                break;
            } else {
                std::cout << "Hero not found" << std::endl;
                selectLoop++;
            }
        } else {
            std::cout << "Invalid input" << std::endl;
            selectLoop++;
        }
    }
    std::cout << std::endl;

}

void Interface::battle(){
    std::cout << "You are now playing as " << heroes[_currHero].getName() << std::endl;

    // Main loop
    while(1){
        std::cout << "Do you want to fight or exit?" << std::endl;
        std::cout << "Press (0) to fight" << std::endl;
        std::cout << "Press (9) to exit" << std::endl;
        std::string input;
        std::cin >> input;
        std::cout << std::endl;

        if(input == "9"){
            break;

        } else if(input == "0"){
            // Fight an enemy
            std::cout << "Enemies available: " << std::endl;
            for(int i = 0; i < enemies.size(); i++) {
                std::cout << "(" << i << ") " << enemies[i].getName() << std::endl;
            }
            int name;
            bool found = false;
            std::cout << "Press the corresponding number to fight enemy: ";
            std::cin >> name;
            std::cout << std::endl;
            if(name >= 0 && name < enemies.size()){
                _currEnemy = name;
                found = true;
            }

            if(found){
                std::cout << heroes[_currHero].getName() << "(" << heroes[_currHero].getHp() << " hp" << ")" << " vs. "
                          << enemies[_currEnemy].getName() << "(" << enemies[_currEnemy].getHp() << " hp" << ")" << std::endl;
                int heroDamage = heroes[_currHero].getStrength();
                int enemyHp = enemies[_currEnemy].getHp();
                int enemyDamage = enemies[_currEnemy].getStrength();
                int heroHp = heroes[_currHero].getHp();
                std::cin.ignore();
                while(1){
                    std::cout << "Press enter to continue";
                    std::cin.ignore();
                    // Hero attacks enemy
                    enemyHp -= heroDamage;
                    std::cout << heroes[_currHero].getName() << " attacks " << enemies[_currEnemy].getName() << " for " << heroDamage << " damage" << std::endl;
                    std::cout << enemies[_currEnemy].getName() << " has " << enemyHp << " hp" << std::endl;
                    if(enemyHp <= 0){
                        std::cout << "You won \n" << std::endl;
                        query.prepare("SELECT xp "
                                      "FROM enemy "
                                      "WHERE name = :name");
                        query.bindValue(":name", QString::fromStdString(enemies[_currEnemy].getName()));
                        query.exec();
                        while(query.next()){
                            heroes[_currHero].getXp(query.value(0).toInt());
                        }
                        break;
                    }

                    // Enemy attacks hero
                    heroHp -= enemyDamage;
                    std::cout << enemies[_currEnemy].getName() << " attacks " << heroes[_currHero].getName() << " for " << enemyDamage << " damage" << std::endl;
                    std::cout << heroes[_currHero].getName() << " has " << heroHp << " hp" << std::endl;

                    if(heroHp <= 0){
                        std::cout << "You died" << std::endl;
                        break;
                    }
                }
            } else {
                std::cout << "Enemy not found" << std::endl;
            }
        } else {
            std::cout << "Invalid input" << std::endl;
        }
    }
}
