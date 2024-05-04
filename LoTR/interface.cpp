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
                    "gold INT,"
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

    if (!query.exec("CREATE TABLE IF NOT EXISTS cave ("
                    "cave_id INT NOT NULL AUTO_INCREMENT,"
                    "name CHAR(125),"
                    "gold INT NOT NULL,"
                    "PRIMARY KEY (cave_id)"
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

    query.exec("SELECT * FROM cave");
    if(query.size() < 1){
        // Execute SQL commands to insert data into the enemy table
        if (!query.exec("INSERT INTO cave (name, gold) "
                        "VALUES ('Weak cave', 1),"
                        "('Normal cave', 2),"
                        "('Hard cave', 3),"
                        "('Dangerous cave', 4),"
                        "('Impossible cave', 5),"
                        "('Boss cave', 6),"
                        "('Farmers cave', 0)")) {
            qWarning() << "Failed to insert data into table 'cave':";
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
            std::cout << std::endl;
            std::cout << "You are now playing as " << heroes[_currHero].getName() << std::endl;
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
            for(int i = 0; i < heroes.size(); i++) {
                if(name == std::to_string(i)) {
                    _currHero = i;
                    std::cout << std::endl;
                    std::cout << "You are now playing as " << heroes[_currHero].getName() << std::endl;
                    return true;

                }
            }
                std::cout << "Hero not found" << std::endl;
                selectLoop++;
            } else {
                std::cout << "Invalid input" << std::endl;
                selectLoop++;
            }
        }
    return true;
}

bool Interface::singleBattle(){
    // Main loop
    while(1){
        std::cout << std::endl;
        // Shows available enemies
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
            if(enemies[_currEnemy].getName() == "Dragon"){
                std::cout << "You have encountered the Dragon!" << std::endl;
                std::cout << "The final battle has begun!" << std::endl;
                std::cout << std::endl;
                _bossBattle = true;
            }
        }

        if(found){
            heroes[_currHero].fight(enemies[_currEnemy]);
            if(heroes[_currHero].getHp() <= 0){
                query.exec("SELECT * FROM hero WHERE name = '" + QString::fromStdString(heroes[_currHero].getName()) + "'");
                while(query.next()){
                    heroes[_currHero].setHp(query.value(2).toInt());
                }
                _bossBattle = false;
                return false;
            } else if(_bossBattle){
                return false;
            } else {
                return true;
            }
        } else {
            std::cout << "Enemy not found" << std::endl;
        }
    }
}

bool Interface::caveBattle(){
    std::cout << std::endl;
    int cavePicked;
    while(1){
        // Shows available caves
        std::cout << "Caves available: " << std::endl;
        query.exec("SELECT * FROM cave");
        int number = 0;
        while(query.next()){
            std::cout << "(" << number << ") " << query.value(1).toString().toStdString() << std::endl;
            number++;
        }

        std::cout << "Press the corresponding number to enter cave: ";
        std::cin >> cavePicked;
        std::cout << std::endl;
        if(cavePicked >= 0 && cavePicked < query.size()){
            if(cavePicked == 5){
                std::cout << "This is the boss level" << std::endl;
                std::cout << "Are you sure you want to enter? There is no turning back (y/n): ";
                std::string input;
                std::cin >> input;
                if(input == "y"){
                    break;
                } else {
                    continue;
                }
            }
            break;
        } else {
            std::cout << "Invalid cave number" << std::endl;
        }
    }

    Cave cave(cavePicked+1);
    std::vector<Enemy> caveEnemies = cave.makeCave();
    for(int i = 0; i < caveEnemies.size(); i++){
        if(caveEnemies[i].getName() == "Dragon"){
            std::cout << "You have fought your way to the dragons lair!" << std::endl;
            std::cout << std::endl;
            _bossBattle = true;
        }
        heroes[_currHero].fight(caveEnemies[i]);
        if(cavePicked != 5){
            std::cout << "You have fought bravely, but there are more enemies ahead!" << std::endl;
            std::cout << "You can still turn back and exit the cave" << std::endl;
            if(i < caveEnemies.size()-1){
                std::cout << "The next enemy is: " << caveEnemies[i+1].getName() << std::endl;
                std::cout << "Press (0) to venture further into the cave" << std::endl;
                std::cout << "Press (1) to exit the cave" << std::endl;
                std::string input;
                std::cin >> input;
                std::cout << std::endl;
                if(input == "1"){
                    return true;
                }
            }
        }
        if(heroes[_currHero].getHp() <= 0){
            query.exec("SELECT * FROM hero WHERE name = '" + QString::fromStdString(heroes[_currHero].getName()) + "'");
            while(query.next()){
                heroes[_currHero].setHp(query.value(2).toInt());
            }
            _bossBattle = false;
            return false;
        }
    }
    std::cout << "You have cleared the cave!" << std::endl;
    std::cout << "You have earned " << cave.getGold() << " gold" << std::endl;
    std::cout << std::endl;
    query.exec("UPDATE hero SET gold = gold + '" +
               QString::number(cave.getGold()) +
               "' WHERE name = '" +
               QString::fromStdString(heroes[_currHero].getName()) + "'");
    heroes[_currHero].getGold(cave.getGold());
    return true;
}

bool Interface::gameChoice(){
    std::cout << "Do you want to fight or exit?" << std::endl;
    std::cout << "Press (0) to fight" << std::endl;
    std::cout << "Press (1) to go to caves" << std::endl;
    std::cout << "Press (9) to exit" << std::endl;
    std::cout << "Enter your choice: ";
    std::string input;
    std::cin >> input;

    if(input == "0"){
        if(singleBattle()){
            return true;
        }
        return false;
    } else if(input == "1"){
        if(caveBattle()){
            return true;
        }
        return false;
    } else if(input == "9"){
        return false;
    }
    return true;
}

void Interface::gameLoop(){
    if(!heroSelection()){
        return;
    }

    while(gameChoice()){}

    if(_bossBattle){
        std::cout << "Congratulations you have defeated the Dragon!" << std::endl;
    }
}
