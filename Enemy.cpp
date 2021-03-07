#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "GameCharacter.h"
#include "Player.h"

#include "Enemy.h"

using namespace std;

Enemy::Enemy() : GameCharacter("Noname", Tag::ENEMY) {}
Enemy::Enemy(string n, int h, int a, int d) : GameCharacter(n, Tag::ENEMY, h, a, d, 0) {}
Enemy::Enemy(string n, int h, int a, int d, int v) : GameCharacter(n, Tag::ENEMY, h, a, d, v) {}

bool Enemy::triggerEvent(Object* o) {
    Player* p = dynamic_cast<Player*>(o);
    int playerDealt = p->dealDamage(*this);
    cout << "You deal " << playerDealt << " damage!";
    cout << string(min(playerDealt / 20, 100), '!') << endl;

    if (!checkIsDead()) {
        int playerTaken = dealDamage(*p);
        cout << "You take " << playerTaken << " damage from " << name;
        cout << string(min(playerTaken / 20 + 1, 100), '!');
        cout << " (" << p->health << " / " << p->maxHealth << ')' << endl << endl;
        return false;
    }
    else {
        cout << "You have slain an enemy!" << endl;
        if (value > 0) cout << "Gained $" << value << "!" << endl;
        else if (value < 0) cout << "Lost $" << -value << "! QAQ" << endl;
        p->value += value;
        cout << endl;
        return true;
    }
}
