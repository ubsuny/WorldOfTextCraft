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


  battle.splash();
  std::cout << "Are you ready to begin? [Y/n] " << std::endl;
  char c;
  std::cin >> c;
  if ( c == 'y' || c == 'Y') {
    std::cout << "Excellent... let us begin..." << std::endl;
  } else {
    std::cout << "Oh well. Another time then..." << std::endl;
    return 0;
  }


  battle.printActions(); 

  

  // Now loop and FIGHT!
  do {
    battle.print();
    std::cout << "Continue? [Y/n] " << std::endl;
    std::cin >> c;
    if ( c != 'y' && c != 'Y') {
      std::cout << "Bye, then." << std::endl;
      break;
    } 
  }
  while ( battle.performScriptedActions() );


  std::cout << "   ___                _ _                  _ " << std::endl;
  std::cout << "  / _ \\___   ___   __| | |__  _   _  ___  / \\" << std::endl;
  std::cout << " / /_\\/ _ \\ / _ \\ / _` | '_ \\| | | |/ _ \\/  /" << std::endl;
  std::cout << "/ /_\\\\ (_) | (_) | (_| | |_) | |_| |  __/\\_/ " << std::endl;
  std::cout << "\\____/\\___/ \\___/ \\__,_|_.__/ \\__, |\\___\\/   " << std::endl;
  std::cout << "                              |___/          " << std::endl;


  return 0; 
};
