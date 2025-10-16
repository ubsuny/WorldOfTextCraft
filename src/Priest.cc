#include "Priest.h"

int Priest::defend(Character* other) {
    return defaultDefend(other); 
}

int Priest::heal(Character* other) {
    return defaultHeal(other); 
}

int Priest::attack(Character* other) {
    return defaultAttack(other); 
}

