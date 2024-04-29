#include <iostream>

#include "enemy.h"
#include "hero.h"
#include "interface.h"

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
    Interface Interface;

    std::cout << "Welcome to the game \n" << std::endl;
    Interface.heroSelection();
    Interface.battle();

    return 0;
}
