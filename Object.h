#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class Tag {
    NONE, PLAYER, NPC, ENEMY, ITEM, FOUNTAIN
};

class Object {
public:
    string name;
    Tag tag;
    Object();
    Object(string, Tag);

    /* pure virtual function */
    virtual bool triggerEvent(Object*) = 0;
};

#endif // OBJECT_H_INCLUDED
