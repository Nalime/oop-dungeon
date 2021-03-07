#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
#include "Object.h"
#include "NPC.h"
#include "Enemy.h"
#include "Item.h"
#include "Fountain.h"
#include "Room.h"
#include "RandomUtil.h"

#include "Generator.h"

using namespace std;

vector<Room> Generator::generateRooms(int w, int h, int l) {
    vector<Room> ret;
    ret.reserve(w * h);

    vector<vector<bool>> visited(h, vector<bool>(w));
    vector<vector<Room>> roomMap(h, vector<Room>(w, Room(false)));

    int startX = randInt(0, h / 3);
    int startY = randInt(0, w / 3);
    bool flipX = randInt(0, 1), flipY = randInt(0, 1);
    int dx = 1, dy = 1;
    if (flipX) startX = h - startX - 1, dx = -1;
    if (flipY) startY = w - startY - 1, dy = -1;
    int ttl = l;

    // do a dfs-like procedure to draw the room layout

    auto makeDoors = [&](int x, int y, pair<int, int> dir) {
        int nx = x + dir.first, ny = y + dir.second;
        if (dir == pair<int, int>{0, 1}) { // RIGHT
            roomMap[x][y].rightRoom = &roomMap[nx][ny];
            roomMap[nx][ny].leftRoom = &roomMap[x][y];
        }
        else if (dir == pair<int, int>{-1, 0}) { // UP
            roomMap[x][y].upRoom = &roomMap[nx][ny];
            roomMap[nx][ny].downRoom = &roomMap[x][y];
        }
        else if (dir == pair<int, int>{0, -1}) { // LEFT
            roomMap[x][y].leftRoom = &roomMap[nx][ny];
            roomMap[nx][ny].rightRoom = &roomMap[x][y];
        }
        else { // DOWN
            roomMap[x][y].downRoom = &roomMap[nx][ny];
            roomMap[nx][ny].upRoom = &roomMap[x][y];
        }
    };

    function<void(int, int, int, int)> dfs = [&](int x, int y, int dx, int dy) -> void {
        if (!ttl) return;
        ttl--;

        double roll = randDouble(0, 5 - 5.0 * ttl / l);
        while (roll < 1) {
            int totalWeight = abs(dx) + abs(dy);
            if (!totalWeight) {
                dx = randInt(0, 1) ? -1 : 1;
                dy = randInt(0, 1) ? -1 : 1;
            }

            int nx = x, ny = y;
            int xdir = 0, ydir = 0;
            int intRoll = randInt(1, totalWeight);
            if (intRoll <= abs(dx)) xdir = copysign(1, dx);
            else ydir = copysign(1, dy);
            nx += xdir, ny += ydir;

            bool outOfBounds = nx < 0 || nx >= h || ny < 0 || ny >= w;
            if (!outOfBounds && !visited[nx][ny]) {
                int ndx = dx, ndy = dy;
                if (randRoll() < 0.3) dx += randInt(-2, 2);
                if (randRoll() < 0.3) dy += randInt(-2, 2);
                visited[x][y] = true;

                makeDoors(x, y, { xdir, ydir });

                dfs(nx, ny, ndx, ndy);
            }
            else {
                if (!outOfBounds && randInt(0, 1)) makeDoors(x, y, { xdir, ydir });

                if (randInt(0, 1)) dx += randInt(-2, 2);
                else dy += randInt(-2, 2);
            }

            roll = roll * 1.5 + 0.1;
        }
    };

    visited[startX][startY] = true;
    dfs(startX, startY, dx, dy);

    // do bfs to turn the room "martix" into a room array
    queue<pair<int, int>> q;
    map<Room*, Room*> mapp{ {nullptr, nullptr} };

    visited.clear();
    visited.resize(h, vector<bool>(w));

    q.push({ startX, startY });
    visited[startX][startY] = true;
    int idx = 0;
    while (!q.empty()) {
        int x = q.front().first, y = q.front().second;
        q.pop();

        Room& r = roomMap[x][y];
        ret.push_back(r);
        ret.back().index = idx++;
        mapp[&r] = &ret.back();

        if (r.rightRoom != nullptr && !visited[x][y + 1]) {
            q.push({ x, y + 1 });
            visited[x][y + 1] = true;
        }

        if (r.upRoom != nullptr && !visited[x - 1][y]) {
            q.push({ x - 1, y });
            visited[x - 1][y] = true;
        }

        if (r.leftRoom != nullptr && !visited[x][y - 1]) {
            q.push({ x, y - 1 });
            visited[x][y - 1] = true;
        }

        if (r.downRoom != nullptr && !visited[x + 1][y]) {
            q.push({ x + 1, y });
            visited[x + 1][y] = true;
        }
    }

    // prints layout for debugging
    //for (int i = 0; i < h; i++) {
    //    for (int j = 0; j < w; j++) {
    //        if (visited[i][j]) {
    //            if (startX == i && startY == j) cout << '*';
    //            else cout << (mapp[&roomMap[i][j]]->index + 9) / 10;
    //        }
    //        else cout << ' ';
    //    }
    //    cout << endl;
    //}

    for (auto& i : ret) {
        i.upRoom = mapp[i.upRoom];
        i.downRoom = mapp[i.downRoom];
        i.leftRoom = mapp[i.leftRoom];
        i.rightRoom = mapp[i.rightRoom];
    }

    for (int i = 1; i < (int)ret.size() - 1; i++) {
        ret[i].objects = generateRoomObjects();
    }

    Enemy* boss = generateEnemy();
    boss->name = "[BOSS] " + boss->name;
    boss->maxHealth = boss->maxHealth * 2 + 100;
    boss->health = boss->maxHealth;
    boss->attack = boss->attack * 6 / 5 + 10;
    boss->defense = copysign(log10(max(1, abs(boss->defense))), boss->defense) * 20 + 20;

    ret.back().isExit = true;
    ret.back().objects.push_back(boss);

    return ret;
}

vector<Object*> Generator::generateRoomObjects() {
    vector<Object*> ret;

    try {
        // 15% NPC, 45% Monster
        double mobRoll = randRoll();
        if (mobRoll < 0.15) {
            ret.push_back(generateNPC());
        }
        else {
            if (mobRoll < 0.6) {
                ret.push_back(generateEnemy());
            }
            else if (mobRoll > 0.8) {
                ret.push_back(generateFountain());
            }

            // 30% of at least one item if no NPC
            double itemRoll = randRoll();
            while (itemRoll < 0.3) {
                ret.push_back(generateItem());
                itemRoll = itemRoll * 2.5 + 0.05;
            }
        }
    }
    catch (string s) {
        cout << "A HORRIBLE ERROR OCCURRED." << endl;
        cout << s << " generator list is empty, random generation failed!" << endl;
        cout << "ABORTING..." << endl;
        cout << "-> INPUT ANY KEY TO QUIT <-" << endl;
        string t;
        cin >> t;
        exit(87);
    }

    return ret;
}

Item* Generator::generateItem() {
    if (items.empty()) throw string("Item");
    int mn = 1, mx = items.rbegin()->first;
    int roll = randInt(mn, mx);
    return new Item(items.lower_bound(roll)->second);
}

NPC* Generator::generateNPC() {
    if (npcs.empty()) throw string("NPC");
    int mn = 1, mx = npcs.rbegin()->first;
    int roll = randInt(mn, mx);
    NPC* npc = new NPC(npcs.lower_bound(roll)->second);
    if (npc->isCommodityRandom) {
        double roll = randRoll();
        while (roll < 0.8) {
            Item* ip = generateItem();
            npc->commodity.push_back(*ip);
            delete ip;
            roll = roll * 1.2 + 0.02;
        }
    }
    else {
        random_shuffle(npc->commodity.begin(), npc->commodity.end());
    }
    return npc;
}

Enemy* Generator::generateEnemy() {
    if (enemies.empty()) throw string("Enemy");
    int mn = 1, mx = enemies.rbegin()->first;
    int roll = randInt(mn, mx);
    return new Enemy(enemies.lower_bound(roll)->second);
}

Fountain* Generator::generateFountain() {
    return new Fountain();
}

void Generator::initialize() {
    ifstream sItem("!items.txt"), sNPC("!npcs.txt"), sEnemy("!enemies.txt");

    string s;
    int wItem = 0;
    while (getline(sItem, s)) {
        if (s.substr(0, 7) == "name = ") {
            int w, h, a, d, v;

            sItem.ignore(48763, '=');
            sItem >> w;
            sItem.ignore(48763, '=');
            sItem >> h;
            sItem.ignore(48763, '=');
            sItem >> a;
            sItem.ignore(48763, '=');
            sItem >> d;
            sItem.ignore(48763, '=');
            sItem >> v;

            string name = s.substr(8, s.find_last_of('"') - 8);
            Item item(name, h, a, d, v);
            wItem += max(1, w);
            items[wItem] = item;
        }
    }

    int wEnemy = 0;
    while (getline(sEnemy, s)) {
        if (s.substr(0, 7) == "name = ") {
            int w, h, a, d, v;

            sEnemy.ignore(48763, '=');
            sEnemy >> w;
            sEnemy.ignore(48763, '=');
            sEnemy >> h;
            sEnemy.ignore(48763, '=');
            sEnemy >> a;
            sEnemy.ignore(48763, '=');
            sEnemy >> d;
            sEnemy.ignore(48763, '=');
            sEnemy >> v;

            string name = s.substr(8, s.find_last_of('"') - 8);
            Enemy enemy(name, h, a, d, v);
            wEnemy += max(1, w);
            enemies[wEnemy] = enemy;
        }
    }

    int wNPC = 0;
    while (getline(sNPC, s)) {
        if (s.substr(0, 7) == "name = ") {
            int w;
            string script, flavor, commodity;
            bool isRnd = true;

            sNPC.ignore(48763, '=');
            sNPC >> w;
            sNPC.ignore(48763, '=');
            getline(sNPC, script);
            sNPC.ignore(48763, '=');
            getline(sNPC, flavor);
            sNPC.ignore(48763, '=');
            getline(sNPC, commodity);

            string name = s.substr(8, s.find_last_of('"') - 8);
            vector<Item> vItems;
            if (commodity.find("custom") != commodity.npos) {
                isRnd = false;
                while (getline(sNPC, s) && s != "endcommodity") {
                    if (s.substr(0, 7) == "name = ") {
                        int h, a, d, v;

                        sNPC.ignore(48763, '=');
                        sNPC >> h;
                        sNPC.ignore(48763, '=');
                        sNPC >> a;
                        sNPC.ignore(48763, '=');
                        sNPC >> d;
                        sNPC.ignore(48763, '=');
                        sNPC >> v;

                        string itemName = s.substr(8, s.find_last_of('"') - 8);
                        vItems.push_back(Item(itemName, h, a, d, v));
                    }
                }
            }

            flavor = flavor.substr(2, (int)flavor.size() - 3);
            script = script.substr(2, (int)script.size() - 3);
            NPC npc(name, script, flavor, isRnd, vItems);
            wNPC += max(1, w);
            npcs[wNPC] = npc;
        }
    }
}