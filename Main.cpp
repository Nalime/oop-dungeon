#include <iostream>
#include <locale>
#include "Dungeon.h"

using namespace std;

int main() {
    for (;;) {
        Dungeon dungeon;;
        dungeon.runDungeon();

        // End?
        string input;
        char o;
        cout << "Play again? (Y/N)" << endl;
        for (;;) {
            cin >> input;
            if (input.empty()) cout << "Invalid response, Y or N only." << endl;
            else {
                o = tolower(input[0]);
                if (o == 'y' || o == 'n') break;
                else cout << "Invalid response, Y or N only." << endl;
            }
        }

        if (o == 'n') {
            cout << "See ya next time!" << endl;
            break;
        }
    }

    return 0;
}
