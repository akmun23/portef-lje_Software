#include "magicshop.h"

MagicShop::MagicShop() {}

bool MagicShop::enterShop(Hero& hero){
    std::cout << std::endl;

    int choice;
    std::cout << "Welcome to the magic shop! Would you like to see our wares?" << std::endl;
    std::cout << "You currently have " << hero.getGold() << " gold" << std::endl;
    std::cout << "Press (0) to see wares" << std::endl;
    std::cout << "Press (1) to get more information on an item" << std::endl;
    std::cout << "Press (2) to see your inventory" << std::endl;
    std::cout << "Press (3) to buy an item" << std::endl;
    std::cout << "Press (9) to leave" << std::endl;
    int counter = 0;
    while(1){
        if(counter != 0){
            std::cout << "Press (4) to get all options" << std::endl;
        }
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cout << std::endl;

        if(choice == 0){
            showWares();
        } else if(choice == 1){
            getInfo();
        } else if(choice == 2){
            showInventory(hero);
        } else if(choice == 3){
            buyItem(hero);
        } else if(choice == 4){
            std::cout << "Press (0) to see wares" << std::endl;
            std::cout << "Press (1) to get more information on an item" << std::endl;
            std::cout << "Press (2) to see your inventory" << std::endl;
            std::cout << "Press (3) to buy an item" << std::endl;
            std::cout << "Press (9) to leave" << std::endl;
        } else if(choice == 9){
            return true;
        } else {
            std::cout << "Invalid choice" << std::endl;
        }
        counter++;
    }
}

void MagicShop::showWares(){
    query.exec("SELECT * FROM magic");
    std::cout << "Magic wares:" << std::endl;
    while(query.next()){
        std::cout << "(" << query.value(0).toString().toStdString() << ") "  << query.value(1).toString().toStdString() << std::endl;
    }
    std::cout << std::endl;
}

void MagicShop::getInfo(){
    std::cout << "Enter the ID of the item you would like more information on or press (0) to go back: ";
    int id;
    std::cin >> id;
    if(id == 0){
        return;
    }
    // Where the element is joined onto the element id
    query.prepare("SELECT magic.*, element.element AS element "
                  "FROM magic "
                  "INNER JOIN element ON magic.element_id = element.element_id "
                  "WHERE magic_id = :magic_id");
    query.bindValue(":magic_id", id);
    query.exec();
    query.next();
    std::cout << "Name: " << query.value(1).toString().toStdString() << std::endl;
    std::cout << "Strength: " << query.value(2).toInt() << std::endl;
    std::cout << "Self harm: " << query.value(3).toInt() << std::endl;
    std::cout << "Magic cost: " << query.value(4).toInt() << std::endl;
    std::cout << "Element: " << query.value(8).toString().toStdString() << std::endl;
    std::cout << "Price: " << query.value(6).toInt() << std::endl;
    //If the requirement is not null show the requirement
    if(!query.value(7).isNull()){
        std::cout << "Requirement: " << query.value(7).toString().toStdString() << std::endl;
    }
    std::cout << std::endl;
}

void MagicShop::showInventory(Hero& hero){
    query.prepare("SELECT magic.magic_id, magic.name "
                  "FROM magic "
                  "INNER JOIN inventory ON magic.magic_id = inventory.magic_id "
                  "INNER JOIN hero ON inventory.hero_id = hero.hero_id "
                  "WHERE hero.name = :hero_name");
    query.bindValue(":hero_name", QString::fromStdString(hero.getName()));
    query.exec();
    std::cout << "Your inventory:" << std::endl;
    while(query.next()){
        std::cout << "(" << query.value(0).toInt() << ") " << query.value(1).toString().toStdString() << std::endl;
    }
    std::cout << std::endl;
}

void MagicShop::buyItem(Hero& hero){
    std::cout << "Enter the ID of the item you would like to buy or press (0) to go back: ";
    query.exec("SELECT * FROM magic");
    int id;
    std::cin >> id;
    if(id == 0){
        std::cout << std::endl;
        return;
    } else if(id < 0 || id > query.size()){
        std::cout << "Invalid ID" << std::endl;
        return;
    }
    // Finds the item based on the id
    query.prepare("SELECT * FROM magic WHERE magic_id = :magic_id");
    query.bindValue(":magic_id", id);
    query.exec();

    // Checks if the hero has enough gold to buy the item
    int price = 0;
    if(query.next()){
        price = query.value(6).toInt();
        if(hero.getGold() < price){
            std::cout << "You do not have enough gold to buy this item" << std::endl;
            std::cout << std::endl;
            return;
        }
    }

    // Checks if the hero already has the item
    query.prepare("SELECT * FROM inventory "
                  "WHERE hero_id = (SELECT hero_id from hero WHERE name = :name) "
                  "AND magic_id = :magic_id");
    query.bindValue(":name", QString::fromStdString(hero.getName()));
    query.bindValue(":magic_id", id);
    query.exec();
    if(query.next()){
        std::cout << "You already have this item" << std::endl;
        std::cout << std::endl;
        return;
    }

    // Checks if the hero meets the requirement for the item
    query.prepare("SELECT required_magic FROM magic WHERE magic_id = :magic_id");
    query.bindValue(":magic_id", id);
    query.exec();
    query.next();
    if(!query.value(0).isNull()){
        std::string requirement = query.value(0).toString().toStdString();
        query.prepare("SELECT * FROM inventory "
                      "WHERE hero_id = (SELECT hero_id from hero WHERE name = :name) "
                      "AND magic_id = (SELECT magic_id from magic WHERE name = :requirement)");
        query.bindValue(":name", QString::fromStdString(hero.getName()));
        query.bindValue(":requirement", QString::fromStdString(requirement));
        query.exec();
        if(!query.next()){
            std::cout << "You do not meet the requirement for this item" << std::endl;
            std::cout << std::endl;
            return;
        }
    }

    // Inserts the item into the inventory and takes the gold from the hero
    query.prepare("INSERT INTO inventory (hero_id, magic_id) "
                  "VALUES ((SELECT hero_id FROM hero WHERE name = :name), :magic_id)");
    query.bindValue(":name", QString::fromStdString(hero.getName()));
    query.bindValue(":magic_id", id);
    query.exec();
    query.next();
    hero.gainGold(-price);

    query.prepare("SELECT * FROM magic WHERE magic_id = :magic_id");
    query.bindValue(":magic_id", id);
    query.exec();
    query.next();
    std::cout << "You have bought " << query.value(1).toString().toStdString() << std::endl;
    std::cout << "You now have " << hero.getGold() << " gold" << std::endl;
    std::cout << std::endl;
}
