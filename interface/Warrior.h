#ifndef Warrior_h
#define Warrior_h

/* __          __             _             */
/* \ \        / /            (_)            */
/*  \ \  /\  / /_ _ _ __ _ __ _  ___  _ __  */
/*   \ \/  \/ / _` | '__| '__| |/ _ \| '__| */
/*    \  /\  / (_| | |  | |  | | (_) | |    */
/*     \/  \/ \__,_|_|  |_|  |_|\___/|_|    */

class Warrior : public Character {
public:
  Warrior(std::string name = "", int defense_power = 0)
      : Character("Warrior", name, 0, defense_power, 0) {};

  int defend(Character *other = 0) override { return defaultDefend(other); }
  int heal(Character *other = 0) override { return defaultHeal(other); }
  int attack(Character *other = 0) override { return defaultAttack(other); }
};

#endif
