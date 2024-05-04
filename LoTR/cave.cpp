#include "cave.h"

Cave::Cave(int caveID):_id(caveID) {
    query.exec("SELECT * FROM cave WHERE cave_id = '" + QString::number(caveID) + "'");
    query.next();
    _name = query.value(1).toString().toStdString();
    _gold = query.value(2).toInt();
}

std::vector<Enemy> Cave::makeCave(){
    std::vector<Enemy> enemies;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(4, 7);
    int numEnemies = dis(gen);

    std::string name;
    int hp;
    int strength;
    int xp;

    if(_id == 7){
        query.exec("SELECT * FROM enemy WHERE enemy_id = '7'");
        query.next();
        name = query.value(1).toString().toStdString();
        hp = query.value(2).toInt();
        strength = query.value(3).toInt();
        xp = query.value(4).toInt();
        for(int i = 0; i < numEnemies; i++){
            enemies.push_back(Enemy(name, hp, strength, xp));
        }
    } else if(_id == 6){
        std::cout << "This is the boss level" << std::endl;
        for(int i = 0; i < numEnemies; i++){
            query.exec("SELECT * FROM enemy WHERE enemy_id = '6'");
            query.next();
            name = query.value(1).toString().toStdString();
            hp = query.value(2).toInt();
            strength = query.value(3).toInt();
            xp = query.value(4).toInt();
            enemies.push_back(Enemy(name, hp, strength, xp));
        }
        query.exec("SELECT * FROM enemy WHERE enemy_id = '8'");
        query.next();
        name = query.value(1).toString().toStdString();
        hp = query.value(2).toInt();
        strength = query.value(3).toInt();
        xp = query.value(4).toInt();
        enemies.push_back(Enemy(name, hp, strength, xp));
    } else {
        for(int i = 0; i < numEnemies; i++){
            if(i == numEnemies-1){
                query.exec("SELECT * FROM enemy WHERE enemy_id = '" + QString::number(_id+2) + "'");
            } else {
                std::uniform_int_distribution<> dis(_id, _id+1);
                int enemyID = dis(gen);
                query.exec("SELECT * FROM enemy WHERE enemy_id = '" + QString::number(enemyID) + "'");
            }
            query.next();
            name = query.value(1).toString().toStdString();
            hp = query.value(2).toInt();
            strength = query.value(3).toInt();
            xp = query.value(4).toInt();
            enemies.push_back(Enemy(name, hp, strength, xp));
        }
    }
    return enemies;
}

int Cave::getGold(){
    return _gold;
}
