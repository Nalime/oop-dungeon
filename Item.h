#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include "Object.h"
#include "Player.h"

using namespace std;

class Player;

class Item : public Object {
public:
    int health, attack, defense, value;

    Item();
    Item(string, int, int, int);
    Item(string, int, int, int, int);

    /* Virtual function that you need to complete    */
    /* In Item, this function should deal with the   */
    /* pick up action. You should add status to the  */
    /* player.                                       */
    bool triggerEvent(Object*);
    string getInfo();
};

#endif // ITEM_H_INCLUDED
