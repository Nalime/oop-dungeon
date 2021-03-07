#ifndef NPC_H_INCLUDED
#define NPC_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include "GameCharacter.h"
#include "Player.h"
#include "Item.h"

using namespace std;

class NPC : public GameCharacter {
private:
    void interact(Player* o);
    void shop(Player* o);
public:
    string script, flavor;
    bool isCommodityRandom;
    vector<Item> commodity;
    NPC();
    NPC(string, string, const vector<Item>&);
    NPC(string, string, string, bool, const vector<Item>&);

    /* Virtual function that you need to complete   */
    /* In NPC, this function should deal with the   */
    /* transaction in easy implementation           */
    bool triggerEvent(Object*);
};

#endif // NPC_H_INCLUDED
