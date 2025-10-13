#ifndef Rogue_h
#define Rogue_h

/*  _____                         */
/* |  __ \                        */
/* | |__) |___   __ _ _   _  ___  */
/* |  _  // _ \ / _` | | | |/ _ \ */
/* | | \ \ (_) | (_| | |_| |  __/ */
/* |_|  \_\___/ \__, |\__,_|\___| */
/*               __/ |            */
/*              |___/             */

class Rogue : public Character {
public:
  Rogue(std::string name = "", int attack_power = 0)
      : Character("Rogue", name, attack_power, 0, 0) {};

  int defend(Character *other = 0) override { return defaultDefend(other); }
  int heal(Character *other = 0) override { return defaultHeal(other); }
  int attack(Character *other = 0) override { return defaultAttack(other); }
};

#endif
