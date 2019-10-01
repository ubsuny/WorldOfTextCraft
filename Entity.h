#ifndef Entity_h
#define Entity_h

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

// --------------------------------------
/*  ______       _   _ _          */
/* |  ____|     | | (_) |         */
/* | |__   _ __ | |_ _| |_ _   _  */
/* |  __| | '_ \| __| | __| | | | */
/* | |____| | | | |_| | |_| |_| | */
/* |______|_| |_|\__|_|\__|\__, | */
/*                          __/ | */
/*                         |___/  */
// --------------------------------------
//
// This is the main base class for the Entities
// in the game.
//
// It provides access to these
// data members:
//  std::string className_;  // Name of this class (like, Warrior or Druid or Rogue)
//  std::string name_;       // Name of this particular entity (like, Lothar the Great)
//  int isMagicUser_;        // Can this user use magic? 
//  int hitPoints_;          // Number of hit points left.
//  int mana_;               // Number of magical "mana" points are left to perform magic
//  int attackPower_;        // Number of hit points inflicted if I attack
//  int defensePower_;       // Number of hit points mitigated if someone attacks ME
//  int healPower_;          // Number of hit points to heal
//  Entity * target_;        // This Entity's current target for action
//
// These will store the history of the results of various actions
//  action_map myAttacks_;
//  action_map myDefends_;
//  action_map myHeals_;
//  action_map myReducedHitPoints_;
//  action_map myIncreasedHitPoints_; 
//
// The interface should be written in the base classes:
//  Attack a target:        virtual int attack( Entity * target=0 );
//  Heal an ally:           virtual int heal  ( Entity * ally=0   );
//  Defend against a target virtual int defend( Entity * target=0 );
//
// These should make appropriate use of the helper functions
//  int reduceHitPoints( int attack );
//  int increaseHitPoints( int heal );
// --------------------------------------
class Entity {

 public:

  Entity( std::string className,
	  std::string name,
	  unsigned int attackPower,
	  unsigned int healPower,
	  unsigned int defensePower,
	  unsigned int maxMana = 0,
	  bool check=true
	 );
  virtual ~Entity();

  // The Battle class will handle the status of everyone in the fight.
  // It will perform turn-based actions. 
  friend class Battle;

  // For recording actions
  typedef std::vector<int>                          action_vector; 
  typedef std::map< unsigned int, action_vector >   action_map;

  // All of these default to "do nothing" and should be overriden in the derived classes. 
  virtual int attack( Entity * target=0 );
  virtual int heal  ( Entity * ally=0   );
  virtual int defend( Entity * target=0 );

  // Getter methods
  std::string className()    const { return className_;} 
  std::string name()         const { return name_; }
  int         attackPower()  const { return attackPower_; }
  int         defensePower() const { return defensePower_; }
  int         healPower()    const { return healPower_; }
  int         hitPoints()    const { return hitPoints_; }
  int         mana()         const { return mana_; }
  bool        isDead()       const { return hitPoints_ <= 0; }
  bool        isAlive()      const { return !isDead(); }

  // Input and output
  virtual void input( std::string instring);
  virtual void input( std::istream &in );
  virtual void print( std::ostream &out) const;
  virtual void printStats( std::ostream &out) const;
  friend std::ostream & operator<<( std::ostream & out, Entity const & e);
  friend std::istream & operator>>( std::istream & in, Entity & e);



  //
  // Call these functions correctly in your derived classes
  // when overloading "attack", "heal", and "block"

  // Reduce the hit points of "this" entity
  int reduceHitPoints( int attack );
  // Increase the hit points of "this" entity
  int increaseHitPoints( int heal );


  // Can this user use magic?
  bool isMagicUser() const { return isMagicUser_; }

  // Set my current target
  void setTarget( Entity * target ) { target_ = target; }

  // Get my current target
  Entity * getTarget( void ) {  return target_; }

  // These will store the history of the results of various actions
  action_map const & myAttacks()            const { return myAttacks_;}
  action_map const & myDefends()            const { return myDefends_;}
  action_map const & myHeals()              const { return myHeals_;}
  action_map const & myReducedHitPoints()   const { return myReducedHitPoints_;}
  action_map const & myIncreasedHitPoints() const { return myIncreasedHitPoints_;}

  // Print the actions in a json format for turn "iturn". 
  void printActions(std::ostream & out, unsigned int iturn) const; 
  
 protected :
  unsigned int turn_;      // Turn that "this" Entity is on.
  std::string className_;  // Name of this class (like, Warrior or Druid or Rogue)
  std::string name_;       // Name of this particular entity (like, Lothar the Great)
  int isMagicUser_;        // Can this user use magic? 
  int hitPoints_;          // Number of hit points left.
  int maxHitPoints_;       // Maximum number of hit points possible 
  int mana_;               // Number of magical "mana" points are left to perform magic
  int attackPower_;        // Number of hit points inflicted if I attack
  int defensePower_;       // Number of hit points mitigated if someone attacks ME
  int healPower_;          // Number of hit points to heal
  

  Entity * target_;        // This Entity's current target for action

  unsigned int getTurn() const { return turn_; } // return this turn.

  bool checkPowers(); // This will check to ensure the input values are sensible



  // Here are some default "attack", "defend", and "heal" methods. 
  int defaultAttack( Entity * target = 0);
  int defaultHeal  ( Entity * target = 0);
  int defaultDefend( Entity * target = 0);

  // These will store the history of the results of various actions
  action_map myAttacks_;
  action_map myDefends_;
  action_map myHeals_;
  action_map myReducedHitPoints_;
  action_map myIncreasedHitPoints_; 
  
  
};

#endif
