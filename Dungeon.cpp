#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <exception>
#include "Player.h"
#include "Enemy.h"
#include "NPC.h"
#include "Room.h"
#include "Record.h"
#include "RandomUtil.h"
#include "Generator.h"

#include "Dungeon.h"

using namespace std;

const static char* saveFilename = "!save.txt";

Dungeon::Dungeon() {}

void Dungeon::createPlayer() {
    string name("???");
    cout << "Enter your name:" << endl;
    cin >> name;
    cout << "Hello " << name << "!\n" << endl;
    player = Player(name, 100, 20, 0);
}

void Dungeon::createMap() {
    rooms = generator.generateRooms(10, 10, 50);
    player.changeRoom(&rooms[0]);
}

set<Direction> Dungeon::getMovementOptions() {
    Room* rp = player.currentRoom;
    set<Direction> ret;

    if (rp->upRoom != nullptr) ret.insert(Direction::UP);
    if (rp->downRoom != nullptr) ret.insert(Direction::DOWN);
    if (rp->leftRoom != nullptr) ret.insert(Direction::LEFT);
    if (rp->rightRoom != nullptr) ret.insert(Direction::RIGHT);

    return ret;
}

void Dungeon::handleMovement(Direction d) {
    Room* rp = player.currentRoom;

    switch (d) {
    case Direction::UP:
        player.changeRoom(rp->upRoom);
        break;
    case Direction::DOWN:
        player.changeRoom(rp->downRoom);
        break;
    case Direction::LEFT:
        player.changeRoom(rp->leftRoom);
        break;
    case Direction::RIGHT:
        player.changeRoom(rp->rightRoom);
        break;
    }
}

void Dungeon::handleEvent(Object* o) {
    if (o->triggerEvent(&player)) {
        if (!player.currentRoom->popObject(o)) {
            cout << "Something terrible happened that you may or may not see" << endl;
            cout << "(Totally a feature)" << endl;
        }
    }
}

void Dungeon::startGame() {
    generator.initialize();
    createPlayer();
    createMap();
}

void Dungeon::chooseAction(vector<Object*>& objects) {
    bool hasEnemy = player.currentRoom->hasEnemy();
    string optionsDialog;

    optionsDialog += "Options:\n";
    set<char> validKeys{ 'q' , 'z'};

    optionsDialog += "Z - Save Game State\n";
    optionsDialog += "Q - Check Player Status\n";

    set<Direction> moveOptions;
    if (!hasEnemy) {
        moveOptions = getMovementOptions();
        if (moveOptions.count(Direction::UP)) {
            optionsDialog += "W - Move Up\n";
            validKeys.insert('w');
        }
        if (moveOptions.count(Direction::DOWN)) {
            optionsDialog += "S - Move Down\n";
            validKeys.insert('s');
        }
        if (moveOptions.count(Direction::LEFT)) {
            optionsDialog += "A - Move Left\n";
            validKeys.insert('a');
        }
        if (moveOptions.count(Direction::RIGHT)) {
            optionsDialog += "D - Move Right\n";
            validKeys.insert('d');
        }
    }
    else {
        optionsDialog += "E - RUN!!!\n";
        validKeys.insert('e');
    }

    vector<Object*> interactOptions(objects);
    interactOptions.erase(remove_if(interactOptions.begin(), interactOptions.end(), [&](Object* o) {
        return o->tag != Tag::ENEMY && hasEnemy;
    }), interactOptions.end());
    sort(interactOptions.begin(), interactOptions.end(), [](Object* a, Object* b) {
        return a->tag < b->tag;
    });

    // not considering size > 9 for now ("key overflow")
    for (int i = 0; i < (int)interactOptions.size(); i++) {
        Object* obj = interactOptions[i];
        stringstream ss;
        ss << i + 1 << " - ";

        static map<Tag, string> flavor{ {Tag::ENEMY, "Attack"}, {Tag::ITEM, "Pick up"}, {Tag::FOUNTAIN, "Drink from"} };
        Tag tag = obj->tag;

        NPC* npc = dynamic_cast<NPC*>(obj);
        if (npc != nullptr) ss << npc->flavor;
        else if (!flavor.count(tag)) ss << "Interact with";
        else ss << flavor[tag];

        ss << ' ' << obj->name;

        Item* item = dynamic_cast<Item*>(obj);
        if (item != nullptr) ss << " ( " << item->getInfo() << ")";

        ss << endl;
        optionsDialog += ss.str();
    }

    char o;
    string input;
    for (;;) {
        cout << optionsDialog;
        cout << "\nInput your option: " << flush;
        cin >> input;
        o = tolower(input[0]);

        if (validKeys.count(o) || (isdigit(o) && stoi(input) > 0 && stoi(input) <= (int)interactOptions.size())) {
            cout << endl;
            break;
        }
        else cout << "Invalid input." << endl;
    }

    static map<char, Direction> dir{
        {'w', Direction::UP},
        {'s', Direction::DOWN},
        {'a', Direction::LEFT},
        {'d', Direction::RIGHT}
    };

    if (o == 'z') {
        ofstream os(saveFilename);
        saveToFile(&player, rooms, os);
        cout << "Save Success!" << endl;
    }
    else if (o == 'q') player.triggerEvent(&player);
    else if (o == 'e') player.changeRoom(player.previousRoom);
    else if (dir.count(o)) handleMovement(dir[o]);
    else handleEvent(interactOptions[stoi(input) - 1]);
}

bool Dungeon::checkGameLogic() {
    Room* rp = player.currentRoom;
    if (player.checkIsDead() || (rp->isExit && !rp->hasEnemy())) return false;

    vector<Object*> objects(rp->objects);
    chooseAction(objects);

    return true;
}

void Dungeon::runDungeon() {
    ifstream is(saveFilename);

    bool loadSave = false;
    if (is.is_open()) {
        string input;
        char o;
        cout << "Save found, load save file? ([Y]es/[N]o/[E]rase)" << endl;
        for (;;) {
            cin >> input;
            if (input.empty()) cout << "Invalid response." << endl;
            else {
                o = tolower(input[0]);
                if (o == 'y') loadSave = true;
                if (o == 'y' || o == 'n' || o == 'e') break;
                else cout << "Invalid response." << endl;
            }
        }

        if (o == 'e') {
            is.close();
            remove(saveFilename);
        }
    }

    if (loadSave) loadFromFile(&player, rooms, is);
    else startGame();

    while (checkGameLogic());

    if (player.checkIsDead()) cout << "You died!" << endl;
    else {
        cout << "You have reached the exit!" << endl;
        cout << "You win!" << endl;
    }

    cout << "\n-> INPUT ANY KEY TO CONTINUE <-" << endl;
    string s;
    cin >> s;

    static vector<string> asciigo = {
        R"(*******************************************************************************)",
        R"(*   ___      ___      ___     _____         ___     _   _    _____    _____   *)",
        R"(*  / _ \    / _ \    /   \   /  ___|       / _ \   | | | |  /  ___|  /  _  \  *)",
        R"(* / / \_\  / | | \  / | | \  | |          / / \ \  | | | |  | |      | | | |  *)",
        R"(* | | __   | |_| |  | | | |  | |___       | | | |  | | | |  | |___   | |_| |  *)",
        R"(* | | \ \  |  _  |  | | | |  |  ___|      | | | |  | | | |  |  ___|  |    _/  *)",
        R"(* \ \_/ /  | | | |  | | | |  | |___       \ \_/ /  \ \_/ /  | |___   | |\ \   *)",
        R"(*  \___/   |_| |_|  |_|_|_|  |_____|       \___/    \___/   |_____|  |_| \_|  *)",
        R"(*                                                                             *)",
        R"(*******************************************************************************)",
    };

    for (string i : asciigo) cout << i << endl;
    cout << endl;
}

