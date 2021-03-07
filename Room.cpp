#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "Object.h"

#include "Room.h"

using namespace std;

int Room::count = 0;

Room::Room() : Room(false, {}) {}
Room::Room(bool e) : Room(e, {}) {}
Room::Room(const vector<Object*>& v) : Room(false, v) {}
Room::Room(bool e, const vector<Object*>& v) : index(count++), isExit(e), objects(v),
upRoom(nullptr), downRoom(nullptr), leftRoom(nullptr), rightRoom(nullptr) {}

bool Room::popObject(Object* o) {
    auto it = find(objects.begin(), objects.end(), o);
    if (it == objects.end()) return false;

    objects.erase(it);
    delete o;
    return true;
}

bool Room::hasEnemy() {
    return any_of(objects.begin(), objects.end(), [](Object* obj) {
        return obj->tag == Tag::ENEMY;
    });
}