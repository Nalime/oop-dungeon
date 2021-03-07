#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Object.h"
#include "Player.h"

#include "Item.h"

using namespace std;

Item::Item() : Object("Noname", Tag::ITEM), health(0), attack(0), defense(0), value(0) {}
Item::Item(string n, int h, int a, int d) : Item(n, h, a, d, 0) {}
Item::Item(string n, int h, int a, int d, int v) : Object(n, Tag::ITEM), health(h), attack(a), defense(d), value(v) {}

/* Virtual function that you need to complete    */
/* In Item, this function should deal with the   */
/* pick up action. You should add status to the  */
/* player.                                       */
bool Item::triggerEvent(Object* o) {
    Player* p = dynamic_cast<Player*>(o);
    if (p == nullptr) return false;
    p->addItem(*this);
    return true;
}

string Item::getInfo() {
    stringstream ss;
    if (!health && !attack && !defense) ss << "USELESS ^_^ HAHA ";
    if (health) ss << "+HP: " << health << ' ';
    if (attack) ss << "+ATK: " << attack << ' ';
    if (defense) ss << "+DEF: " << defense << ' ';
    if (value) ss << "$ " << value << ' ';
    return ss.str();
}
