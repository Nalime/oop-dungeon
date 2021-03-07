#include <iostream>
#include <string>
#include <vector>

#include "Object.h"

using namespace std;

Object::Object() : name("Noname"), tag(Tag::NONE) {}
Object::Object(string n, Tag t) : name(n), tag(t) {}
