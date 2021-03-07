#include <iostream>
#include <locale>
#include <set>
#include <string>
#include <vector>
#include "GameCharacter.h"
#include "Player.h"
#include "Item.h"

#include "NPC.h"

using namespace std;

NPC::NPC() : GameCharacter("Noname", Tag::NPC) {}
NPC::NPC(string n, string s, const vector<Item>& v) : NPC(n, s, "Talk to", true, v) {}
NPC::NPC(string n, string s, string f, bool b, const vector<Item>& v)
    : GameCharacter(n, Tag::NPC, 100, 20, 10), script(s), flavor(f), isCommodityRandom(b), commodity(v) {}

void NPC::interact(Player* p) {
    if (!script.empty()) cout << name << ": " << script << endl;

    set<char> validOptions{'e', '1'};
    string optionsDialog("Options:\nE - Exit\n1 - Shop! >_<\n");

    char c;
    for (;;) {
        cout << endl << optionsDialog << endl;
        cout << "Input your choice: " << flush;
        string input;
        cin >> input;
        c = tolower(input[0]);

        switch (c) {
        case 'e':
            cout << "\nCYA!\n" << endl;
            return;
        case '1':
            shop(p);
            break;
        default:
            cout << "wut pls don't type random things, " << endl;
            cout << "ok?" << endl;
            break;
        }
    }
}

void NPC::shop(Player* p) {
    cout << endl;
    if (commodity.empty()) {
        cout << "Sorry, " << p->name << '.' << endl;
        cout << "But I have nothingggggggggg.. XDDDDDDDDD" << endl;
        return;
    }

    cout << "All MIT!\n" << endl;

    char c;
    for (;;) {
        if (commodity.empty()) {
            cout << "I have nothing now... XD" << endl;
            break;
        }
        cout << "Options:\nE - Left Turn" << endl;
        cout << "Items: (You have $" << p->value << ')' << endl;
        for (int i = 0; i < (int)commodity.size(); i++) {
            cout << i + 1 << " - " << commodity[i].name << ": " << commodity[i].getInfo() << endl;
        }

        cout << "\nInput your choice: " << flush;
        string input;
        cin >> input;
        c = tolower(input[0]);

        if (c == 'e') {
            cout << endl << "okiedokie" << endl;
            break;
        }
        else if (!isdigit(c) || stoi(input) <= 0 || stoi(input) > (int)commodity.size()) {
            cout << "What's " << input << "? Can I eat that?" << endl;
            continue;
        }
        else {
            cout << endl;
            int i = stoi(input) - 1;
            Item item = commodity[i];
            if (p->value < item.value) {
                cout << "You can't afford that.\n" << endl;
            }
            else {
                cout << "-$" << item.value << endl;
                p->value -= item.value;
                p->addItem(item);
                commodity.erase(commodity.begin() + i);
            }
        }
    }
}

bool NPC::triggerEvent(Object* o) {
    Player* p = dynamic_cast<Player*>(o);
    if (p == nullptr) cout << "U R NOT A PLAYER!!!" << endl;
    else interact(p);
    return false;
}
