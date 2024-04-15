#include <iostream>
#include <QCoreApplication>
#include <QtSql>
#include <QSqlDatabase>

#include "enemy.h"
#include "hero.h"

using namespace std;

int main()
{
    // Setup database
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("LoTR"); // Ret til det schema navn du har valgt
    db.setUserName("aksel");  // Ret brugernavn
    db.setPassword("password");  // Ret password
    db.open();

    // Makes query object
    QSqlQuery query;

    // Create enemies from a database containing the data
    query.exec("SELECT * FROM enemy");
    int enemyAmount = query.size();
    std::vector<Enemy> enemies;
    for (int i = 0; i < enemyAmount; i++) {
        query.next();
        std::string name = query.value(1).toString().toStdString();
        int hp = query.value(2).toInt();
        int strength = query.value(3).toInt();
        int xp = query.value(4).toInt();
        enemies.push_back(Enemy(name, hp, strength, xp));
    }

    std::cout << "Welcome to the game" << std::endl;
    std::cout << "Do you want to create a new hero or select an existing one?" << std::endl;

    // Create enemies from a database containing the data
    query.exec("SELECT * "
               "FROM hero");
    int heroAmount = query.size();
    std::vector<Hero> heroes;
    for (int i = 0; i < heroAmount; i++) {
        query.next();
        int hero_id = query.value(0).toInt();
        heroes.push_back(Hero(hero_id));
    }

    int currHero = 0;
    int currEnemy = 0;
    int selectLoop = 0;
    while(1){
        // Give choice of selecting hero or create new one
        if(selectLoop == 0) {
            std::cout << "Type 'New' for new hero or 'Select' for existing hero" << std::endl;
        }

        std::string input;
        std::cin >> input;
        input[0] = tolower(input[0]);

        if(input == "new") {
            std::string name;
            std::cout << "Enter name for hero: " << std::endl;
            std::cin >> name;
            heroes.push_back(Hero(name));
            currHero = heroes.size()-1;
            break;
        } else if(input == "select") {
            if(heroes.size() == 0) {
                std::cout << "No heroes available" << std::endl;
                continue;
            } else {
                std::cout << "Heroes available: ";
                for(int i = 0; i < heroes.size(); i++) {
                    std::cout << heroes[i].getName() << ", ";
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
                    currHero = i;
                    found = true;
                }
            }
            if(found) {
                break;
            } else {
                std::cout << "Hero not found" << std::endl;
            }
        } else {
            std::cout << "Invalid input" << std::endl;
        }
    }

    // Main loop
    while(1){
        std::cout << "Type fight or exit" << std::endl;
        std::string input;
        std::cin >> input;
        input[0] = tolower(input[0]);
        if(input == "exit"){
            break;
        } else if(input == "fight"){
            // Fight an enemy
            std::cout << "Enemies available: ";
            for(int i = 0; i < enemies.size(); i++) {
                std::cout << enemies[i].getName() << std::endl;
            }
            std::string name;
            std::cout << "Enter name of enemy: " << std::endl;
            std::cin.ignore();
            std::getline(std::cin, name);
            bool found = false;
            for(int i = 0; i < enemies.size(); i++) {
                std::string enemyName = enemies[i].getName();
                if(enemyName == name){
                    currEnemy = i;
                    found = true;
                }
            }

            if(found){
                std::cout << heroes[currHero].getName() << " vs. " << enemies[currEnemy].getName() << std::endl;
                std::cout << heroes[currHero].getName() << " has " << heroes[currHero].getHp() << " hp" << std::endl;
                std::cout << enemies[currEnemy].getName() << " has " << enemies[currEnemy].getHp() << " hp" << std::endl;
                int heroDamage = heroes[currHero].getStrength();
                int enemyHp = enemies[currEnemy].getHp();
                int enemyDamage = enemies[currEnemy].getStrength();
                int heroHp = heroes[currHero].getHp();
                while(1){
                    std::cout << "Press enter to continue" << std::endl;
                    std::cin.ignore();
                    // Hero attacks enemy
                    enemyHp -= heroDamage;
                    std::cout << heroes[currHero].getName() << " attacks " << enemies[currEnemy].getName() << " for " << heroDamage << " damage" << std::endl;
                    std::cout << enemies[currEnemy].getName() << " has " << enemyHp << " hp" << std::endl;
                    if(enemyHp <= 0){
                        std::cout << "You won" << std::endl;
                        query.prepare("SELECT xp "
                                      "FROM enemy "
                                      "WHERE name = :name");
                        query.bindValue(":name", QString::fromStdString(enemies[currEnemy].getName()));
                        query.exec();
                        while(query.next()){
                            heroes[currHero].giveXp(query.value(0).toInt());
                        }
                        break;
                    }

                    // Enemy attacks hero
                    heroHp -= enemyDamage;
                    std::cout << enemies[currEnemy].getName() << " attacks " << heroes[currHero].getName() << " for " << enemyDamage << " damage" << std::endl;
                    std::cout << heroes[currHero].getName() << " has " << heroHp << " hp" << std::endl;

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

    return 0;
}
