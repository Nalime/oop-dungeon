#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include "Object.h"

using namespace std;

class Room {
public:
    static int count;

    Room* upRoom;
    Room* downRoom;
    Room* leftRoom;
    Room* rightRoom;
    bool isExit;
    int index;
    /* Contain objects like monsters, NPCs, etc. */
    vector<Object*> objects;

    Room();
    Room(bool);
    Room(const vector<Object*>&);
    Room(bool, const vector<Object*>&);
    /* Pops out the specific object, used when the interaction is done */
    bool popObject(Object*);
    bool hasEnemy();
};

#endif // ROOM_H_INCLUDED
