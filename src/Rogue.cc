#include "Rogue.h"

int Rogue::defend(Character* other) {
    return defaultDefend(other); 
}

int Rogue::heal(Character* other) {
    return defaultHeal(other); 
}

int Rogue::attack(Character* other) {
    return defaultAttack(other); 
}

