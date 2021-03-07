#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <sstream>
#include "Player.h"
#include "Enemy.h"
#include "NPC.h"
#include "Item.h"
#include "Fountain.h"

#include "Record.h"

using namespace std;

void savePlayer(const Player* p, ofstream& os) {
    os << "player" << endl;

    os << "name = \"" << p->name << '\"' << endl;
    os << "health = " << p->health << endl;
    os << "maxHealth = " << p->maxHealth << endl;
    os << "attack = " << p->attack << endl;
    os << "defense = " << p->defense << endl;
    os << "value = " << p->value << endl;
    os << "curRoom = " << (p->currentRoom == nullptr ? -1 : p->currentRoom->index) << endl;
    os << "prevRoom = " << (p->previousRoom == nullptr ? -1 : p->previousRoom->index) << endl;
    os << "inventory = " << endl;

    for (Item i : p->inventory) {
        os << "name = \"" << i.name << '\"' << endl;
        os << "health = " << i.health << endl;
        os << "attack = " << i.attack << endl;
        os << "defense = " << i.defense << endl;
        os << "value = " << i.value << endl;
    }

    os << "endplayer" << endl;
}

void saveRooms(const vector<Room>& r, ofstream& os) {
    os << "rooms" << endl;

    for (Room i : r) {
        os << "id = " << i.index << endl;
        os << "exit = " << i.isExit << endl;

        os << "up = " << (i.upRoom == nullptr ? -1 : i.upRoom->index) << endl;
        os << "down = " << (i.downRoom == nullptr ? -1 : i.downRoom->index) << endl;
        os << "left = " << (i.leftRoom == nullptr ? -1 : i.leftRoom->index) << endl;
        os << "right = " << (i.rightRoom == nullptr ? -1 : i.rightRoom->index) << endl;

        os << "objects = " << endl;
        for (Object* o : i.objects) {
            os << "name = \"" << o->name << '\"' << endl;

            Item* item;
            NPC* npc;
            Enemy* enemy;
            os << "tag = " << (int)o->tag << endl;
            switch (o->tag) {
            case Tag::ITEM:
                os << "ITEM" << endl;
                item = dynamic_cast<Item*>(o);
                if (item == nullptr) break;
                os << "health = " << item->health << endl;
                os << "attack = " << item->attack << endl;
                os << "defense = " << item->defense << endl;
                os << "value = " << item->value << endl;
                break;
            case Tag::NPC:
                os << "NPC" << endl;
                npc = dynamic_cast<NPC*>(o);
                if (npc == nullptr) break;
                os << "script = \"" << npc->script << '\"' << endl;
                os << "flavor = \"" << npc->flavor << '\"' << endl;
                os << "commodity = " << endl;

                for (Item i : npc->commodity) {
                    os << "name = \"" << i.name << '\"' << endl;
                    os << "health = " << i.health << endl;
                    os << "attack = " << i.attack << endl;
                    os << "defense = " << i.defense << endl;
                    os << "value = " << i.value << endl;
                }

                os << "endcommodity" << endl;
                break;
            case Tag::ENEMY:
                os << "ENEMY" << endl;
                enemy = dynamic_cast<Enemy*>(o);
                if (enemy == nullptr) break;
                os << "health = " << enemy->health << endl;
                os << "maxMealth = " << enemy->maxHealth << endl;
                os << "attack = " << enemy->attack << endl;
                os << "defense = " << enemy->defense << endl;
                os << "value = " << enemy->value << endl;
                break;
            }
        }

        os << "endobjects" << endl;
    }

    os << "endrooms" << endl;
}

void loadPlayer(Player* p, vector<Room>& r, ifstream& is) {
    string s;
    if (!getline(is, s) || (!s.empty() && s != "player")) {
        cout << "Save file corrupt. Aborting..." << endl;
        exit(187);
    }

    string name;
    int h, mh, a, d, v, cr, pr;

    is.ignore(48763, '=');
    getline(is, name);
    is.ignore(48763, '=');
    is >> h;
    is.ignore(48763, '=');
    is >> mh;
    is.ignore(48763, '=');
    is >> a;
    is.ignore(48763, '=');
    is >> d;
    is.ignore(48763, '=');
    is >> v;
    is.ignore(48763, '=');
    is >> cr;
    is.ignore(48763, '=');
    is >> pr;

    name = name.substr(2, (int)name.size() - 3);
    p->name = name;
    p->health = h;
    p->maxHealth = mh;
    p->attack = a;
    p->defense = d;
    p->value = v;
    p->changeRoom(&r[cr]);
    if (pr != -1) p->previousRoom = &r[pr];

    is.ignore(48763, '=');
    while (is >> s && s != "endplayer") {
        string iname;
        int ih, ia, id, iv;

        is.ignore(48763, '=');
        getline(is, iname);

        is.ignore(48763, '=');
        is >> ih;

        is.ignore(48763, '=');
        is >> ia;

        is.ignore(48763, '=');
        is >> id;

        is.ignore(48763, '=');
        is >> iv;

        iname = iname.substr(2, (int)iname.size() - 3);
        p->inventory.push_back(Item(iname, ih, ia, id, iv));
    }

    is.ignore(48763, '\n');
}

void loadRooms(vector<Room>& r, ifstream& is) {
    string s;
    if (!getline(is, s) || (!s.empty() && s != "rooms")) {
        cout << "Save file corrupt. Aborting..." << endl;
        exit(187);
    }

    vector<tuple<int, int, int, int>> adj;
    while (is >> s && s != "endrooms") {
        is.ignore(48763, '=');

        int exit, ur, dr, lr, rr;

        is.ignore(48763, '=');
        is >> exit;
        is.ignore(48763, '=');
        is >> ur;
        is.ignore(48763, '=');
        is >> dr;
        is.ignore(48763, '=');
        is >> lr;
        is.ignore(48763, '=');
        is >> rr;

        vector<Object*> objects;
        is.ignore(48763, '=');
        while (is >> s && s != "endobjects") {
            string name;
            int tag;

            is.ignore(48763, '=');
            getline(is, name);

            is.ignore(48763, '=');
            is >> tag;

            name = name.substr(2, (int)name.size() - 3);

            Object* object;
            if (tag == (int)Tag::ITEM) {
                int h, a, d, v;

                is.ignore(48763, '=');
                is >> h;
                is.ignore(48763, '=');
                is >> a;
                is.ignore(48763, '=');
                is >> d;
                is.ignore(48763, '=');
                is >> v;

                object = new Item(name, h, a, d, v);
            }
            else if (tag == (int)Tag::NPC) {
                string script, flavor;

                is.ignore(48763, '=');
                getline(is, script);
                is.ignore(48763, '=');
                getline(is, flavor);

                script = script.substr(2, (int)script.size() - 3);
                flavor = flavor.substr(2, (int)flavor.size() - 3);

                vector<Item> commodity;
                is.ignore(48763, '=');
                while (is >> s && s != "endcommodity") {
                    string iname;
                    int ih, ia, id, iv;

                    is.ignore(48763, '=');
                    getline(is, iname);
                    is.ignore(48763, '=');
                    is >> ih;
                    is.ignore(48763, '=');
                    is >> ia;
                    is.ignore(48763, '=');
                    is >> id;
                    is.ignore(48763, '=');
                    is >> iv;

                    iname = iname.substr(2, (int)iname.size() - 3);
                    commodity.push_back(Item(iname, ih, ia, id, iv));
                }

                object = new NPC(name, script, flavor, false, commodity);
            }
            else if (tag == (int)Tag::ENEMY) {
                int h, mh, a, d, v;

                is.ignore(48763, '=');
                is >> h;
                is.ignore(48763, '=');
                is >> mh;
                is.ignore(48763, '=');
                is >> a;
                is.ignore(48763, '=');
                is >> d;
                is.ignore(48763, '=');
                is >> v;

                Enemy* enemy = new Enemy(name, mh, a, d, v);
                enemy->health = h;
                object = enemy;
            }
            else if (tag == (int)Tag::FOUNTAIN) {
                object = new Fountain();
            }

            objects.push_back(object);
        }

        r.push_back(Room(exit, objects));
        adj.emplace_back(ur, dr, lr, rr);
        is.ignore(48763, '\n');
    }

    for (int i = 0; i < (int)r.size(); i++) {
        int upIdx = get<0>(adj[i]);
        int downIdx = get<1>(adj[i]);
        int leftIdx = get<2>(adj[i]);
        int rightIdx = get<3>(adj[i]);
        if (upIdx != -1) r[i].upRoom = &r[upIdx];
        if (downIdx != -1) r[i].downRoom = &r[downIdx];
        if (leftIdx != -1) r[i].leftRoom = &r[leftIdx];
        if (rightIdx != -1) r[i].rightRoom = &r[rightIdx];
    }

    is.ignore(48763, '\n');
}


void saveToFile(const Player* p, const vector<Room>& r, ofstream& os) {
    saveRooms(r, os);
    savePlayer(p, os);
}

void loadFromFile(Player* p, vector<Room>& r, ifstream& is) {
    loadRooms(r, is);
    loadPlayer(p, r, is);
}
