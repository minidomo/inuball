
#ifndef I_GAMEOBJECT_H
#define I_GAMEOBJECT_H

#include "../common.h"

struct iGameObject {
    virtual bool interact_primary(Node *player) { return false; }
    virtual bool interact_secondary(Node *player) { return false; }
};

#endif
