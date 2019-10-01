#ifndef Boss_h
#define Boss_h

#include "Entity.h"
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
// In addition to the standard Entity interface, it also has
// an "attackAll" function that will attack all of the entities
// inside of a vector<Entity*>. 
// --------------------------------------                      
                      
class Boss : public Entity{
 public :

    friend class Battle; 
  
    Boss( std::string name="", 
	  int attackPower=0, 
	  int healPower=0,
	  int defensePower=0,
	  int mana=0,
	  int multiAttackPower=0 );

    // Default defend, heal, and attack
    virtual int defend( Entity * other =0){
      return defaultDefend(other);
    }
    virtual int heal( Entity * other=0 ){
      return defaultHeal(other);
    }
    virtual int attack( Entity * other=0 ){
      return defaultAttack( other );
    }

    // Special multiattack to attack many at once. 
    virtual int multiAttack( Entity * other = 0);

    virtual void printStats(std::ostream &out) const;
    virtual void print(std::ostream &out) const;


   // Overload the input method.
   virtual void input( std::string line );

 protected: 
   action_map myMultiAttacks_;
   
 private :
   int multiAttackPower_;    // The attack power of a mob who can attack more than one opponent. 


};

#endif
