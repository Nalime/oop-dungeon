#ifndef GAMECHARACTER_H_INCLUDED
#define GAMECHARACTER_H_INCLUDED

#include <iostream>
#include <string>
#include "Object.h"

using namespace std;

class GameCharacter : public Object
{
public:
    int maxHealth;
    int health;
    int attack;
    int defense;
    int value;

    GameCharacter();
    GameCharacter(string, Tag);
    GameCharacter(string, Tag, int, int, int);
    GameCharacter(string, Tag, int, int, int, int);

    bool checkIsDead();
    int takeDamage(int);
    int dealDamage(GameCharacter&);
};

#endif // GAMECHARACTER_H_INCLUDED
