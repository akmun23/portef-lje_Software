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
    query.exec("SELECT * FROM enemy");
    for (int i = 0; i < enemyAmount; i++) {
        query.next();
        std::string name = query.value(1).toString().toStdString();
        int hp = query.value(2).toInt();
        int strength = query.value(3).toInt();
        int xp = query.value(4).toInt();
        enemies.push_back(Enemy(name, hp, strength, xp));
    }

    // Create hero from a database containing the data
    Hero hero("Frodo");

    return 0;
}
