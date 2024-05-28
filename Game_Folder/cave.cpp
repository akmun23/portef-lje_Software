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
    std::string element;

    if(_id == 7){
        query.exec("SELECT * FROM enemy WHERE name = 'Unicorn'");
        query.next();
        name = query.value(1).toString().toStdString();
        hp = query.value(2).toInt();
        strength = query.value(3).toInt();
        xp = query.value(4).toInt();
        element = query.value(5).toString().toStdString();
        for(int i = 0; i < numEnemies; i++){
            enemies.push_back(Enemy(name, hp, strength, xp, element));
        }
    } else if(_id == 6){
        std::cout << "This is the boss level" << std::endl;
        for(int i = 0; i < numEnemies; i++){
            query.exec("SELECT * FROM enemy WHERE name = 'Ape King'");
            query.next();
            name = query.value(1).toString().toStdString();
            hp = query.value(2).toInt();
            strength = query.value(3).toInt();
            xp = query.value(4).toInt();
            element = query.value(5).toString().toStdString();
            enemies.push_back(Enemy(name, hp, strength, xp, element));
        }
        query.exec("SELECT * FROM enemy WHERE enemy_id = 'Dragon'");
        query.next();
        name = query.value(1).toString().toStdString();
        hp = query.value(2).toInt();
        strength = query.value(3).toInt();
        xp = query.value(4).toInt();
        element = query.value(5).toString().toStdString();
        enemies.push_back(Enemy(name, hp, strength, xp, element));
    } else {
        for(int i = 0; i < numEnemies; i++){
            int offset = _id - 1;
            if(i == numEnemies-1){
                query.prepare("SELECT * FROM enemy ORDER BY strength ASC LIMIT 1 OFFSET :offset");
                query.bindValue(":offset", offset+2);
            } else {
                std::uniform_int_distribution<> dis(0, 1);
                int enemyID = offset + dis(gen);
                query.prepare("SELECT * FROM enemy ORDER BY strength ASC LIMIT 1 OFFSET :enemyID");
                query.bindValue(":enemyID", enemyID);
            }
            query.exec();
            query.next();
            name = query.value(1).toString().toStdString();
            hp = query.value(2).toInt();
            strength = query.value(3).toInt();
            xp = query.value(4).toInt();
            element = query.value(5).toString().toStdString();
            enemies.push_back(Enemy(name, hp, strength, xp, element));
        }
    }
    return enemies;
}

int Cave::getGold(){
    return _gold;
}
