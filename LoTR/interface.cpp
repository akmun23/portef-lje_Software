#include "interface.h"

Interface::Interface(QSqlDatabase &db) {
    // Open the database
    if (!db.open()) {
        qWarning() << "Failed to connect to database:";
        qWarning() << db.lastError().text();
    }

    // Check if the database exists
    QSqlQuery query;
    if (!query.exec("CREATE DATABASE IF NOT EXISTS LoTR")) {
        qWarning() << "Failed to create database:";
        qWarning() << query.lastError().text();
    } else {
        qDebug() << "Database created or already exists.";
    }

    // Switch to using the LoTR database
    db.setDatabaseName("LoTR");
    if(!db.open()) {
        qWarning() << "Failed to switch to database:";
        qWarning() << db.lastError().text();
    } else {
        qDebug() << "Using database: LoTR";
    }

    if(!query.exec("CREATE TABLE IF NOT EXISTS hero ("
                    "hero_id INT NOT NULL AUTO_INCREMENT,"
                    "name CHAR(125),"
                    "hp INT NOT NULL,"
                    "strength INT NOT NULL,"
                    "level INT NOT NULL,"
                    "xp INT,"
                    "PRIMARY KEY (hero_id)"
                    ")")) {
        qWarning() << "Failed to create table 'hero':";
        qWarning() << query.lastError().text();
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS enemy ("
                    "enemy_id INT NOT NULL AUTO_INCREMENT,"
                    "name CHAR(125),"
                    "hp INT NOT NULL,"
                    "strength INT NOT NULL,"
                    "xp INT NOT NULL,"
                    "PRIMARY KEY (enemy_id)"
                    ")")) {
        qWarning() << "Failed to create table 'enemy':";
        qWarning() << query.lastError().text();
    }

    qDebug() << "Tables created!";

    query.exec("SELECT * FROM enemy");
    if(query.size() < 1){
        // Execute SQL commands to insert data into the enemy table
        if (!query.exec("INSERT INTO enemy (name, hp, strength, xp) "
                        "VALUES ('Horse', 4, 1, 100),"
                        "('Weak Goblin', 4, 2, 200),"
                        "('Strong Goblin', 8, 3, 400),"
                        "('Stronger Goblin', 10, 4, 500),"
                        "('Strongest Goblin', 15, 5, 800),"
                        "('Ape King', 30, 5, 1000),"
                        "('Unicorn', 5, 8, 1500),"
                        "('Dragon', 100, 10, 3000)")) {
            qWarning() << "Failed to insert data into table 'enemy':";
            qWarning() << query.lastError().text();
        }
        qDebug() << "Data inserted successfully!";
    }

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
        std::string  name = query.value(1).toString().toStdString();
        int hp = query.value(2).toInt();
        int strength = query.value(3).toInt();

        heroes.push_back(Hero(hero_id, name, hp, strength));
    }

}

bool Interface::heroSelection(){
    int selectLoop = 0;
    while(1){
        // Give choice of selecting hero or create new one
        if(selectLoop == 0) {
            std::cout << "Do you want to create a new Hero or select an existing one?" << std::endl;
            std::cout << "Press (0) to create a new hero" << std::endl;
            std::cout << "Press (1) to select an existing hero" << std::endl;
            std::cout << "Press (9) to exit" << std::endl;
        }
        std::cout << "Enter your choice: ";
        std::string input;
        std::cin >> input;
        std::cout << std::endl;
        if(input == "9") {
            return false;
        } else if(input == "0") {
            std::string name;
            std::cout << "Enter name for hero: ";
            std::cin >> name;
            heroes.push_back(Hero(name));
            _currHero = heroes.size()-1;
            return true;
        } else if(input == "1") {
            if(heroes.size() == 0) {
                std::cout << "No heroes available" << std::endl;
                continue;
            } else {
                std::cout << "Heroes available: " << std::endl;
                for(int i = 0; i < heroes.size(); i++) {
                    std::cout << "(" << i << ") " << heroes[i].getName() << std::endl;
                }
            }
            std::string name;
            std::cout << "Enter number for the hero: ";
            std::cin >> name;
            bool found = false;
            for(int i = 0; i < heroes.size(); i++) {
                if(name == std::to_string(i)) {
                    _currHero = i;
                    found = true;
                    return true;
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

void Interface::singleBattle(){
    // Main loop
    while(1){
        std::cout << "Do you want to fight or exit?" << std::endl;
        std::cout << "Press (0) to fight" << std::endl;
        std::cout << "Press (9) to exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::string input;
        std::cin >> input;
        if(input == "9"){
            break;
        } else if(input == "0"){
            std::cout << std::endl;
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

void Interface::gameLoop(){
    if(!heroSelection()){
        return;
    }
    std::cout << "You are now playing as " << heroes[_currHero].getName() << std::endl;
    singleBattle();
}
