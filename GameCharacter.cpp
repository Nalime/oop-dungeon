#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include "Object.h"
#include "RandomUtil.h"

#include "GameCharacter.h"

using namespace std;

GameCharacter::GameCharacter() : Object(), maxHealth(20), health(20), attack(10), defense(0), value(0)  {}
GameCharacter::GameCharacter(string n, Tag t) : GameCharacter(n, t, 20, 10, 0) {}
GameCharacter::GameCharacter(string n, Tag t, int h, int a, int d) : GameCharacter(n, t, h, a, d, 0) {}
GameCharacter::GameCharacter(string n, Tag t, int h, int a, int d, int v) :
    Object(n, t), maxHealth(h), health(h), attack(a), defense(d), value(v) {}

bool GameCharacter::checkIsDead() { return health <= 0; }
int GameCharacter::takeDamage(int d) {
    int rnd = -randInt(min(defense, 0), 0) - randInt(0, max(defense, 0));
    int lostHP = max(1, d + rnd);
    health = max(0ll, (long long)health - lostHP);
    return lostHP;
}

int GameCharacter::dealDamage(GameCharacter& c) {
    int atkMin = (attack * 4ll + 4) / 5;
    int atkMax = min((long long)numeric_limits<int>::max, attack * 6ll / 5);
    return c.takeDamage(randInt(atkMin, atkMax));
}
