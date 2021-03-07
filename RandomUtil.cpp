#include <random>
#include <utility>

#include "RandomUtil.h"

using namespace std;

random_device rd;
mt19937* gen = nullptr;

int randInt(int l, int r) {
    if (gen == nullptr) gen = new mt19937(rd());
    if (l > r) swap(l, r);
    uniform_int_distribution<> dist(l, r);
    return dist(*gen);
    return 0;
}

double randDouble(double l, double r) {
    if (gen == nullptr) gen = new mt19937(rd());
    if (l > r) swap(l, r);
    uniform_real_distribution<> dist(l, r);
    return dist(*gen);
    return 0;
}

double randRoll() { return randDouble(0.0, 1.0); }
