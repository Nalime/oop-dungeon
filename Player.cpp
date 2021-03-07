#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "GameCharacter.h"
#include "Item.h"
#include "Room.h"

#include "Player.h"

using namespace std;

Player::Player() : GameCharacter("Noname", Tag::PLAYER), currentRoom(nullptr), previousRoom(nullptr) {}
Player::Player(string n, int h, int a, int d)
    : GameCharacter(n, Tag::PLAYER, h, a, d, 0), currentRoom(nullptr), previousRoom(nullptr) {}
Player::Player(string n, int h, int a, int d, int v)
    : GameCharacter(n, Tag::PLAYER, h, a, d, v), currentRoom(nullptr), previousRoom(nullptr) {}

void Player::addItem(Item item) {
    cout << "Got " << item.name << "!\n" << endl;
    addStats(item.health, item.attack, item.defense);
    inventory.push_back(item);
}

void Player::addStats(int h, int a, int d) {
    stringstream ss;
    if (h) {
        ss << "HP " << (h > 0 ? '+' : '-') << abs(h) << ' ';
        maxHealth += h;
        health += h;
    }
    if (a) {
        ss << "ATK " << (a > 0 ? '+' : '-') << abs(a) << ' ';
        attack += a;
    }
    if (d) {
        ss << "DEF " << (d > 0 ? '+' : '-') << abs(d) << ' ';
        defense += d;
    }
    cout << ss.str() << '\n' << endl;
};

void Player::changeRoom(Room* r) {
    previousRoom = currentRoom;
    currentRoom = r;

    if (r->isExit) cout << "You entered the exit room!" << endl;
    else cout << "You entered a room, which floor is carved with a big number \"" << r->index << "\"." << endl;

    if (r->hasEnemy()) {
        if (r->isExit) {
            cout << "-> !! HIGH ENERGY WARNING !! <-" << endl;
            cout << "The boss is in front of you!" << endl;
        }
        else cout << "-> An enemy is in your way! <-" << endl;
    }
    cout << endl;
}

/* Virtual function that you need to complete   */
/* In Player, this function should show the     */
/* status of player.                            */
bool Player::triggerEvent(Object* o) {
    cout << "---->  S T A T U S  <----" << endl;
    cout << "Player " << name << ":  $" << value << endl;
    cout << "Health: " << health << " / " << maxHealth << endl;
    cout << "Attack: " << attack << " Defense: " << defense << endl << endl;
    return false;
}
