#ifndef Priest_h
#define Priest_h

#include "Character.h"

/*  _____      _           _    */
/* |  __ \    (_)         | |   */
/* | |__) | __ _  ___  ___| |_  */
/* |  ___/ '__| |/ _ \/ __| __| */
/* | |   | |  | |  __/\__ \ |_  */
/* |_|   |_|  |_|\___||___/\__| */

class Priest : public Character {
public:
  Priest(std::string name = "", int heal_power = 0)
      : Character("Priest", name, 0, 0, heal_power, 100) {};

  // In a heal, we increase the hit points
  int defend(Character *other = 0) override;
  int heal(Character *other = 0) override;
  int attack(Character *other = 0) override;

private:
};

#endif
