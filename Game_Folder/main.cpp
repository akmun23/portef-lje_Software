#include <iostream>

#include "enemy.h"
#include "hero.h"
#include "interface.h"

using namespace std;

int main()
{
    // Replace with your MySQL database connection parameters
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("mysql");
    db.setUserName("aksel");  // Ret brugernavn
    db.setPassword("password");  // Ret password

    // Makes query object
    Interface Interface(db);

    std::cout << "Welcome to the game \n" << std::endl;
    Interface.gameLoop();
    std::cout << "Thank you for playing!" << std::endl;

    return 0;
}
