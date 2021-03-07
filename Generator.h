#ifndef GENERATOR_H_INCLUDED
#define GENERATOR_H_INCLUDED

#include <map>
#include <vector>
#include "Object.h"
#include "NPC.h"
#include "Enemy.h"
#include "Item.h"
#include "Fountain.h"
#include "Room.h"

using namespace std;

class Generator {
public:
    map<int, NPC> npcs;
    map<int, Enemy> enemies;
    map<int, Item> items;

    vector<Room> generateRooms(int, int, int);
    vector<Object*> generateRoomObjects();
    Item* generateItem();
    NPC* generateNPC();
    Enemy* generateEnemy();
    Fountain* generateFountain();
    void initialize();
};

#endif // GENERATOR_H_INCLUDED
