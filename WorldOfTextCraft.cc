#include "Entity.h"

#include "Warrior.h"
#include "Rogue.h"
#include "Priest.h"
#include "Boss.h"

#include "Battle.h"

int main( int argc, char ** argv ) {

  if ( argc < 4 ) {
    std::cout << "Usage : " << argv[0] << " player_config boss_config boss_script   [optional: logfile]" << std::endl;
    return 0; 
  }

  

  Battle battle;

  battle.setUserTurns(true); 

  bool success = true; 

  // Read the party configurations
  std::cout << "reading PC configuration" << std::endl;
  success = battle.readPCConfiguration( argv[1] );
  if ( ! success ) {
    std::cout << "No PC configuration read" << std::endl;
    return 0;
  }
  std::cout << "reading NPC configuration" << std::endl;
  success = battle.readNPCConfiguration( argv[2] );
  if ( ! success ) {
    std::cout << "No PC configuration read" << std::endl;
    return 0;
  }

  // Read a test sequence. 
  std::cout << "reading NPC action script" << std::endl;
  success = battle.loadActionScript( argv[3] );
  if ( ! success ) {
    std::cout << "No NPC script read" << std::endl;
    return 0;
  }


  battle.splash();
  std::cout << "Are you ready to begin? [Y/n] " << std::endl;
  char c;
  std::cin >> c;
  if ( c == 'y' || c == 'Y') {
    std::cout << "Excellent... let us begin..." << std::endl;

    battle.printActions(); 

  

    // Now loop and FIGHT!
    success= true;
    do {
      battle.print();

      success = battle.performUserActions(); 

    }
    while ( success );

  }

  std::string logname = (argc >= 5) ? argv[4] : "battlelog.txt";
  std::ofstream logfile(logname);
  battle.printLog(logfile); 
  

  std::cout << "   ___                _ _                  _ " << std::endl;
  std::cout << "  / _ \\___   ___   __| | |__  _   _  ___  / \\" << std::endl;
  std::cout << " / /_\\/ _ \\ / _ \\ / _` | '_ \\| | | |/ _ \\/  /" << std::endl;
  std::cout << "/ /_\\\\ (_) | (_) | (_| | |_) | |_| |  __/\\_/ " << std::endl;
  std::cout << "\\____/\\___/ \\___/ \\__,_|_.__/ \\__, |\\___\\/   " << std::endl;
  std::cout << "                              |___/          " << std::endl;


  return 0; 
};
