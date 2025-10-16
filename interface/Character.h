#ifndef Character_h
#define Character_h

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <random>

// ----------------------------------------------------------------//
/* _________ .__                                __                 */
/* \_   ___ \|  |__ _____ ____________    _____/  |_  ___________  */
/* /    \  \/|  |  \\__  \\_  __ \__  \ _/ ___\   __\/ __ \_  __ \ */
/* \     \___|   Y  \/ __ \|  | \// __ \\  \___|  | \  ___/|  | \/ */
/*  \______  /___|  (____  /__|  (____  /\___  >__|  \___  >__|    */
/*         \/     \/     \/           \/     \/          \/        */
// ----------------------------------------------------------------//
//
// This is the main base class for the Characters
// in the game. 
// The class is pure virtual. You must implement the following functions in 
// derived classes:
//  virtual int attack(Character *target = 0) = 0;
//  virtual int heal(Character *ally = 0) = 0;
//  virtual int defend(Character *target = 0) = 0;
//
// Data members:
//  std::string class_name_;  // Name of this class (like, Warrior or Druid or
//  Rogue) std::string name_;       // Name of this particular character (like,
//  Lothar the Great) int is_magic_user_;        // Can this user use magic? int
//  hit_points_;          // Number of hit points left. int mana_; // Number of
//  magical "mana" points are left to perform magic int attack_power_;        //
//  Number of hit points inflicted if I attack int defense_power_;       //
//  Number of hit points mitigated if someone attacks ME int heal_power_; //
//  Number of hit points to heal Character * target_;        // This Character's
//  current target for action
//
// These will store the history of the results of various actions
//  ActionMap my_attacks_;
//  ActionMap my_defends_;
//  ActionMap my_heals_;
//  ActionMap my_reduced_hit_points_;
//  ActionMap my_increased_hit_points_;
//
// The interface should be written in the base classes:
//  Attack a target:        virtual int attack( Character * target=0 );
//  Heal an ally:           virtual int heal  ( Character * ally=0   );
//  Defend against a target virtual int defend( Character * target=0 );
//
// These should make appropriate use of the helper functions
//  int reduceHitPoints( int attack );
//  int increaseHitPoints( int heal );
// --------------------------------------
class Character {

public:
  Character(std::string class_name, std::string name, unsigned int attack_power,
            unsigned int heal_power, unsigned int defense_power,
            unsigned int max_mana = 0, bool check = true);
  virtual ~Character();

  // The Battle class will handle the status of everyone in the fight.
  // It will perform turn-based actions.
  friend class Battle;

  // For recording actions
  typedef std::vector<int> ActionVector;
  typedef std::map<unsigned int, ActionVector> ActionMap;

  // All of these default to "do nothing" and should be overriden in the derived
  // classes.
  virtual int attack(Character *target = 0) = 0;
  virtual int heal(Character *ally = 0) = 0;
  virtual int defend(Character *target = 0) = 0;

  // Getter methods
  std::string className() const { return class_name_; }
  std::string name() const { return name_; }
  int attackPower() const { return attack_power_; }
  int defensePower() const { return defense_power_; }
  int healPower() const { return heal_power_; }
  int hitPoints() const { return hit_points_; }
  int mana() const { return mana_; }
  bool isDead() const { return hit_points_ <= 0; }
  bool isAlive() const { return !isDead(); }

  // Input and output
  virtual void input(std::string instring);
  virtual void input(std::istream &in);
  virtual void post_input() {}; // Hook for actions after input, i.e. after stats are set
  virtual void print(std::ostream &out) const;
  virtual void printStats(std::ostream &out) const;
  friend std::ostream &operator<<(std::ostream &out, Character const &e);
  friend std::istream &operator>>(std::istream &in, Character &e);

  //
  // Call these functions correctly in your derived classes
  // when overloading "attack", "heal", and "block"

  // Reduce the hit points of "this" character
  int reduceHitPoints(int attack);
  // Increase the hit points of "this" character
  int increaseHitPoints(int heal);

  // Can this user use magic?
  bool isMagicUser() const { return is_magic_user_; }

  // Set my current target
  void setTarget(Character *target) { target_ = target; }

  // Get my current target
  Character *getTarget(void) { return target_; }

  // These will store the history of the results of various actions
  ActionMap const &myAttacks() const { return my_attacks_; }
  ActionMap const &myDefends() const { return my_defends_; }
  ActionMap const &myHeals() const { return my_heals_; }
  ActionMap const &myReducedHitPoints() const { return my_reduced_hit_points_; }
  ActionMap const &myIncreasedHitPoints() const {
    return my_increased_hit_points_;
  }

  // Print the actions in a json format for turn "iturn".
  void printActions(std::ostream &out, unsigned int iturn) const;

protected:
  unsigned int turn_; // Turn that "this" Character is on.
  std::string
      class_name_;    // Name of this class (like, Warrior or Druid or Rogue)
  std::string name_; // Name of this particular character (like, Lothar the Great)
  int is_magic_user_;  // Can this user use magic?
  int hit_points_;    // Number of hit points left.
  int max_hit_points_; // Maximum number of hit points possible
  int mana_;        // Number of magical "mana" points are left to perform magic
  int attack_power_; // Number of hit points inflicted if I attack
  int defense_power_; // Number of hit points mitigated if someone attacks me
  int heal_power_;    // Number of hit points to heal

  Character *target_; // This Character's current target for action

  unsigned int getTurn() const { return turn_; } // return this turn.

  bool checkPowers(); // This will check to ensure the input values are sensible

  // Here are some default "attack", "defend", and "heal" methods.
  int defaultAttack(Character *target = 0);
  int defaultHeal(Character *target = 0);
  int defaultDefend(Character *target = 0);

  // These will store the history of the results of various actions
  ActionMap my_attacks_;
  ActionMap my_defends_;
  ActionMap my_heals_;
  ActionMap my_reduced_hit_points_;
  ActionMap my_increased_hit_points_;
};

#endif
