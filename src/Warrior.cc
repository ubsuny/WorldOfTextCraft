#include "Warrior.h"

int Warrior::defend(Character* other) {
    return defaultDefend(other); 
}

int Warrior::heal(Character* other) {
    return defaultHeal(other); 
}

int Warrior::attack(Character* other) {
    return defaultAttack(other); 
}

