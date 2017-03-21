#include "Entity.h"

#include "Warrior.h"
#include "Rogue.h"
#include "Priest.h"
#include "Boss.h"


int main( int argc, char ** argv ) {

  Warrior warrior("Larry", 3);
  Priest priest("Moe", 5);
  Rogue rogue("Curly", 15);
  Boss boss("Shemp", 15, 4, 2, 100, 8);

  unsigned i = 0;
  bool done = false;

  boss.setTarget( &priest );
  warrior.setTarget( &boss );
  rogue.setTarget( &boss );
  priest.setTarget( &warrior );

  std::vector<Entity *> party;
  party.push_back( &warrior );
  party.push_back( &rogue );
  party.push_back( &priest );
  

  while ( ! done ) {
    std::cout << "Round " << i << std::endl;
    std::cout << warrior << std::endl;
    std::cout << priest << std::endl;
    std::cout << rogue << std::endl;
    std::cout << boss << std::endl;

    if ( i % 5 == 0 )
      for ( std::vector<Entity *>::iterator it = party.begin();
	    it != party.end(); ++it ) {
	boss.multiAttack( *it );
      }
    else
      boss.attack();
    warrior.defend();
    rogue.attack();
    priest.heal();
    ++i;
    done = (warrior.isDead() && priest.isDead() && rogue.isDead() ) || boss.isDead();
  }


  if ( warrior.isAlive() && priest.isAlive() && rogue.isAlive()) {
    std::cout << "The party wins! The gods rejoice in your ferociousness!" << std::endl;
  } else {
    std::cout << "Your quest has failed. May you drink and fight long in the mead halls of Valhalla" << std::endl;
  }

  return 0; 
};
