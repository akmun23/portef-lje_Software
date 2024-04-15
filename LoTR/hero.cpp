#include "hero.h"

Hero::Hero(std::string name):_name(name) {

}

void Hero::levelUp() {
    _level++;
    _strength++;
    _hp += 2;
    _xp -= 1000;
}
