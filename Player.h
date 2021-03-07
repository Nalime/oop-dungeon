#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include "GameCharacter.h"
#include "Room.h"
#include "Item.h"

using namespace std;

class Item;

class Player : public GameCharacter {
public:
    Room* currentRoom;
    Room* previousRoom;
    vector<Item> inventory;

    Player();
    Player(string, int, int, int);
    Player(string, int, int, int, int);

    void addItem(Item);
    void addStats(int, int, int);
    void changeRoom(Room*);

    /* Virtual function that you need to complete   */
    /* In Player, this function should show the     */
    /* status of player.                            */
    bool triggerEvent(Object*);
};

#endif // PLAYER_H_INCLUDED
