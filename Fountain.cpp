#include <algorithm>
#include "Object.h"
#include "GameCharacter.h"

#include "Fountain.h"

using namespace std;

Fountain::Fountain() : Object("Fountain", Tag::FOUNTAIN) {}

bool Fountain::triggerEvent(Object* o) {
    GameCharacter* c = dynamic_cast<GameCharacter*>(o);
    if (c == nullptr) return false;
    int heal = min(c->maxHealth / 5, c->maxHealth - c->health);
    c->health += heal;
    cout << "Refreshing! One sip one sweet! +" << heal << "HP (";
    cout << c->health << " / " << c->maxHealth << ")\n" << endl;
    return true;
}