#include "Entity.h"

#include "Warrior.h"
#include "Rogue.h"
#include "Priest.h"
#include "Boss.h"

#include "Battle.h"

int main( int argc, char ** argv ) {

  Battle battle;

  bool success = true; 

  // Read the party configurations
  success = battle.readPCConfiguration( "ThreeStooges.txt" );
  if ( ! success ) {
    std::cout << "No PC configuration read" << std::endl;
    return 0;
  }
  success = battle.readNPCConfiguration( "Shemp.txt");
  if ( ! success ) {
    std::cout << "No PC configuration read" << std::endl;
    return 0;
  }


  // Read a test sequence. 
  success = battle.loadActionScript("ShempAttacks.txt");
  if ( ! success ) {
    std::cout << "No NPC script read" << std::endl;
    return 0;
  }
  success = battle.loadActionScript("StoogesBattle.txt");
  if ( ! success ) {
    std::cout << "No PC script read" << std::endl;
    return 0;
  }

  battle.printActions(); 
  
  std::cout << battle.description() << std::endl;

  // Now loop and FIGHT!
  do {
    battle.print();
  }
  while ( battle.performActions() );

  std::cout << "Finished... exiting." << std::endl;

  return 0; 
};
