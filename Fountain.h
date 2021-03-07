#ifndef FOUNTAIN_H_INCLUDED
#define FOUNTAIN_H_INCLUDED

#include "Object.h"

using namespace std;

class Fountain : public Object {
public:
    Fountain();
    /* Virtual function that you need to complete    */
    /* In Item, this function should deal with the   */
    /* pick up action. You should add status to the  */
    /* player.                                       */
    bool triggerEvent(Object*);
};

#endif // FOUNTAIN_H_INCLUDED
