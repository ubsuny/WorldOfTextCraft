#ifndef Boss_h
#define Boss_h

#include "Character.h"
#include <vector>

/*  ____                 */
/* |  _ \                */
/* | |_) | ___  ___ ___  */
/* |  _ < / _ \/ __/ __| */
/* | |_) | (_) \__ \__ \ */
/* |____/ \___/|___/___/ */
//
// This is the Boss class.
// Bosses are special entities that are stronger,
// and have the ability to attack the entire opposing party at once.
//
// In addition to the standard Character interface, it also has
// an "attackAll" function that will attack all of the entities
// inside of a vector<Character*>.
// --------------------------------------

class Boss : public Character {
public:
  friend class Battle;

  Boss(std::string name = "", int attack_power = 0, int heal_power = 0,
       int defense_power = 0, int mana = 0, int multi_attack_power = 0);

  // Default defend, heal, and attack
  int defend(Character *other = 0) override { return defaultDefend(other); }
  int heal(Character *other = 0) override { return defaultHeal(other); }
  int attack(Character *other = 0) override { return defaultAttack(other); }

  // Special multiattack to attack many at once.
  virtual int multiAttack(Character *other = 0);

  void printStats(std::ostream &out) const override;
  void print(std::ostream &out) const override;

  // Overload the input method.
  void input(std::string line) override;

protected:
  ActionMap my_multi_attacks_;

private:
  int multi_attack_power_; // The attack power of a mob who can attack more than
                         // one opponent.
};

#endif
