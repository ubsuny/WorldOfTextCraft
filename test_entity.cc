#include "Entity.h"

#include "Warrior.h"
#include "Rogue.h"
#include "Priest.h"


int main( int argc, char ** argv ) {

  Warrior warrior("Larry", 10);
  Priest priest("Moe", 5);
  Rogue rogue("Curly", 15);

  unsigned i = 0;
  bool done = false; 

  rogue.setTarget( &priest );
  priest.setTarget( &warrior );
  rogue.attack();

  while ( ! done ) {
    std::cout << "Round " << i << std::endl;
    std::cout << warrior << std::endl;
    std::cout << priest << std::endl;
    std::cout << rogue << std::endl;

    warrior.defend( &rogue );
    rogue.attack();
    priest.heal();
    ++i;
    done = warrior.isDead() || priest.isDead() || i >= 20;
  }


  if ( warrior.isAlive() && priest.isAlive() ) {
    std::cout << "The party wins! The gods rejoice in your ferociousness!" << std::endl;
  } else {
    std::cout << "Your quest has failed. May you drink and fight long in the mead halls of Valhalla" << std::endl;
  }

  return 0; 
};
