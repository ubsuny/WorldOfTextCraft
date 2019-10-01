#include "Battle.h"
#include "MatchingHelpers.h"
#include <sstream>

void Battle::splash( std::ostream & out ) const {

  out << "|--------------------------------------------|" <<  std::endl;
  out << "|--------------------------------------------|" <<  std::endl;
  out << "|--------------------------------------------|" <<  std::endl;
  out << "|--------------------------------------------|" <<  std::endl;
  out << "|        __    __           _     _         |" <<  std::endl;
  out << "|       / / /\\ \\ \\___  _ __| | __| |        |" <<  std::endl;
  out << "|       \\ \\/  \\/ / _ \\| '__| |/ _` |        |" <<  std::endl;
  out << "|        \\  /\\  / (_) | |  | | (_| |        |" <<  std::endl;
  out << "|         \\/  \\/ \\___/|_|  |_|\\__,_|        |" <<  std::endl;
  out << "|                                           |" <<  std::endl;
  out << "|                       __                  |" <<  std::endl;
  out << "|                 ___  / _|                 |" <<  std::endl;
  out << "|                / _ \\| |_                  |" <<  std::endl;
  out << "|               | (_) |  _|                 |" <<  std::endl;
  out << "|                \\___/|_|                   |" <<  std::endl;
  out << "|                                           |" <<  std::endl;
  out << "| _____          _     ___           __ _   |" <<  std::endl;
  out << "|/__   \\_____  _| |_  / __\\ __ __ _ / _| |_ |" <<  std::endl;
  out << "|  / /\\/ _ \\ \\/ / __|/ / | '__/ _` | |_| __||" <<  std::endl;
  out << "| / / |  __/>  <| |_/ /__| | | (_| |  _| |_ |" <<  std::endl;
  out << "| \\/   \\___/_/\\_\\\\__\\____/_|  \\__,_|_|  \\__||" <<  std::endl;
  out << "|                                           |" <<  std::endl;
  out << "|--------------------------------------------|" <<  std::endl;
  out << "|--------------------------------------------|" <<  std::endl;
  out << "|--------------------------------------------|" <<  std::endl;

  out << "Welcome brave traveler..." << std::endl;
  out << description_ << std::endl;


}

bool Battle::readPCConfiguration( std::string filename )
{

  if ( pcs_.size() > 0 ) {
    std::cout << "Configuration already read... skipping reading from " << filename << std::endl;
    return false;
  } else {
    std::ifstream in(filename);

    // While not the end of file, keep looping
    while ( ! in.eof() ) {
      std::string line;
      std::getline(in,line);

      // First check if the line is empty or a comment (starts with "!")
      if ( (line.size() > 0 && line[0] == '!') || line == ""  ){
	continue;
      }
      else {

	// Otherwise, we have a valid line. 
	// First read the type from the first token, and create the appropriate class (Warrior, Priest, Rogue).
	// Then read in the stats from the rest of the line. 
	std::stringstream tokens(line);
	std::string entityType;
	std::getline( tokens, entityType, ';' );
      
	std::shared_ptr<Entity> entity;
	if ( entityType == "Warrior" ) {
	  entity = std::shared_ptr<Entity> ( new Warrior() );
	} else if ( entityType == "Priest" ) {
	  entity = std::shared_ptr<Entity> ( new Priest() );
	} else if ( entityType == "Rogue" ) {
	  entity = std::shared_ptr<Entity> ( new Rogue() );
	} else {
	  std::cout << "Entity Type not recognized: " << line << ", skipping" << std::endl;
	  continue;
	}
	entity->input( tokens );
	pcs_.push_back( entity );
	std::cout << "Added entity: " << *( pcs_.back() ) << std::endl;
      }
    }

    in.close();
  }
  return true;
}



bool Battle::readNPCConfiguration( std::string filename )
{
  if ( npcs_.size() > 0 ) {
    std::cout << "Configuration already read... skipping reading from " << filename << std::endl;
    return false;
  } else {
    std::ifstream in(filename);
    while ( ! in.eof() ) {
      std::string line;
      std::getline(in,line);

      if ( (line.size() > 0 && line[0] == '!') || line == ""  ){
	continue;
      }
      else {
	// The first line is the description.
	if ( description_ == "" ) {
	  description_ = line;
	} else {
	  std::shared_ptr<Entity> boss (new Boss() );
	  boss->input( line );
	  npcs_.push_back( boss );
	}
      }
    }
  }
  return true;
}


// Load a bunch of actions from a script
bool Battle::loadActionScript( std::string filename ) 
{  
  if ( filename == "" ) {
    std::cout << "No script file input." << std::endl;
    return false; 
  } else {
    std::ifstream in( filename );

    std::string line;
    bool success = true; 
    while ( !in.eof() && success ) {

      std::getline( in, line );      
      if ( line[0] == '!' || line == "" ) continue;
      QuickAction qa;
      success = parseAction( line, qa );

      if( !success ) {
	std::cout << "Error in parseAction!" << std::endl;
	return false; 
      }

      // If the script has any PC actions, it is scripted and not user-input
      coll_type::iterator pcIt = std::find_if( pcs_.begin(), pcs_.end(), MatchSource( qa.source->name() ) );
      bool found = pcIt != pcs_.end() ;
      if ( found ) {
	scripted_=true;
	std::cout << "Scripting input for " << (*pcIt)->name() << std::endl;
      }
      actions_.push_back(qa);



    }
    in.close();
  }

  return true;

}

// Lines look like this:
// Shemp;attack;Moe;
// Shemp;multiattack;all;
// 1 : find the Entity in the list with "name".
// 2 : find the action they are supposed to do.
// 3 : find the target. If blank, use current target. 
// 4 : Set "name's" target, perform action
bool Battle::parseAction(std::string line, QuickAction & qa)
{

  if ( line == "" ) return false;

  std::vector<std::string> tokens;
  std::stringstream linestream(line);	
  for (std::string each=""; std::getline(linestream, each, ';'); ){
    tokens.push_back(each);
  }

  if ( tokens.size() != 3 ) {
    std::cout << "Improper formatting of line " << line << std::endl;
    return false; 
  } 

  // Initialize both target and source to "error" (pcs_.end() is interpreted as such)
  coll_type::iterator targetIt = pcs_.end(), sourceIt = pcs_.end(); 
  std::shared_ptr<Entity> target;
  std::shared_ptr<Entity> source;
      
  // Find the iterator with name "tokens[0]"
  sourceIt = find_entity( tokens[0] );
  // Check to make sure we found an iterator
  if ( sourceIt == pcs_.end() ) {
    std::cout << "Error processing line " << line << std::endl;
    return false; 
  } 
  source = *sourceIt;

  // Special case of Bosses attacking ALL NPCs
  if ( tokens[2] == "all" ) {
    target = 0; 
    qa.source = source;
    qa.action = MULTIATTACK;
    return true; 
  } 

  // Special case of attacking current target
  if ( tokens[2] == "target" ) {
    target = 0;
  } else {

    // Find the iterator with name "tokens[2]"
    targetIt = find_entity( tokens[2] );
    // Check to make sure we found an iterator
    if ( targetIt == pcs_.end() || targetIt == npcs_.end() ) {
      std::cout << "Error processing line " << line << std::endl;
      return false; 
    }
    target = *targetIt;
  }

  // parse the action type. 
  ActionType actionType = N_ACTIONS; // Error code
  if      ( tokens[1] == "attack") actionType = ATTACK;
  else if ( tokens[1] == "heal") actionType = HEAL;
  else if ( tokens[1] == "defend") actionType = DEFEND;
  else {
    std::cout << "Invalid action " << tokens[1] << std::endl;
    return false; 
  }
  

  // Finally, push back the action
  if ( target ) {
    source->setTarget(target.get());
  }
  qa.source = source;
  qa.action = actionType ;
  return true; 
}


// This will perform all of the actions scripted in "actions_". 
bool Battle::performScriptedActions( ) {

  if ( !scripted_ ) {
    std::cout << "I am expecting a player script, but you didn't give me one." << std::endl;
    return false; 
  }

  if ( !anyPCAlive() ){
    std::cout << "Alas, your party is dead." << std::endl;
    return false; 
  }

  if ( !anyNPCAlive() ) {
    std::cout << "You already won! Hooray!" << std::endl;
    return false; 
  }

  // Special case for first attack : all NPCs will target first PC
  if ( turn_ == 0 && pcs_.size() > 1 ) {
    for ( coll_type::iterator inpc = npcs_.begin();
	  inpc != npcs_.end(); ++inpc ) {
      (*inpc)->setTarget( pcs_.begin()->get() );
    }
  }

  if ( actions_.size() == 0 ){
    std::cout << "No actions to perform" << std::endl;
    return false; 
  }

  for ( coll_type::iterator it = npcs_.begin(); it != npcs_.end(); ++it ) {
    (*it)->turn_ = turn_;
  }

  for ( coll_type::iterator it = pcs_.begin(); it != pcs_.end(); ++it ) {
    (*it)->turn_ = turn_;
  }

  for ( std::vector<QuickAction>::iterator it = actions_.begin();
	it != actions_.end(); ++it ){

    if ( it->source->isDead() ) {
      std::cout << it->source->name() << " is dead" << std::endl;
      continue; 
    }

    // two special cases for bosses: 
    // 1. If their target dies, they pick the next target. 
    // 2. Must check the special case of the "Boss" ability to attack everyone.
    // Since we are storing pointers to the base class, we need to 
    // "dynamic_cast" to the derived class to access its "multi attack"
    Boss * boss = dynamic_cast<Boss *> (it->source.get()); 
    if ( boss != 0 ) {
      // Attack the entire party in a "MultiAttack"
      if ( it->action == MULTIATTACK ) {	
	for ( coll_type::iterator itarget = pcs_.begin();
	      itarget != pcs_.end(); ++itarget ) {
	  boss->multiAttack( itarget->get() );
	}
	continue;
      }
      // If the target is dead, switch to the next in the list.
      if ( boss->getTarget() != 0 && boss->getTarget()->isDead() ) {
	bool bossGotTarget = setBossTarget( boss );
	if ( !bossGotTarget ) {
	  return false; 
	}
      }
    }

    // Now we execute normal attacks. 
    if ( it->action ==  ATTACK ) {
      it->source->attack();
    } else if ( it->action ==  HEAL  ) {
      it->source->heal();
    } else if ( it->action ==  DEFEND ) {
      it->source->defend(); 
    } else {
      std::cout << "Incorrect action! Returning" << std::endl;
      return false; 
    }

    if ( !anyPCAlive() ) {
      std::cout << it->source->name() << " has vanquished your party." << std::endl;
      return false; 
    }


    if ( !anyNPCAlive() ) {
      std::cout << it->source->name() << " has vanquished your foe!!!" << std::endl;
      return false; 
    }

  }


  ++turn_; 

  return anyPCAlive() && anyNPCAlive(); 

}



// This will perform all of the actions scripted in "actions_". 
bool Battle::performUserActions( std::istream & in ) {

  if ( scripted_ ) {
    std::cout << "PC actions already scripted, do not give me scripted actions if you do not want to use them." << std::endl;
    return false; 
  }


  if ( !anyPCAlive() ){
    std::cout << "Alas, your party is dead." << std::endl;
    return false; 
  }

  if ( !anyNPCAlive() ) {
    std::cout << "You already won! Hooray!" << std::endl;
    return false; 
  }

  // Special case for first attack : all NPCs will target first PC
  if ( turn_ == 0 && pcs_.size() > 1 ) {
    for ( coll_type::iterator inpc = npcs_.begin();
	  inpc != npcs_.end(); ++inpc ) {
      (*inpc)->setTarget( pcs_.begin()->get() );
    }
  }

  // Bookkeeping
  if ( actions_.size() == 0 ){
    std::cout << "No actions to perform" << std::endl;
    return false; 
  }
  for ( coll_type::iterator it = npcs_.begin(); it != npcs_.end(); ++it ) {
    (*it)->turn_ = turn_;
  }
  for ( coll_type::iterator it = pcs_.begin(); it != pcs_.end(); ++it ) {
    (*it)->turn_ = turn_;
  }

  // Process the boss actions
  for ( std::vector<QuickAction>::iterator it = actions_.begin();
	it != actions_.end(); ++it ){

    if ( it->source->isDead() ) {
      std::cout << it->source->name() << " is dead" << std::endl;
      continue; 
    }
    // two special cases for bosses: 
    // 1. If their target dies, they pick the next target. 
    // 2. Must check the special case of the "Boss" ability to attack everyone.
    // Since we are storing pointers to the base class, we need to 
    // "dynamic_cast" to the derived class to access its "multi attack"
    Boss * boss = dynamic_cast<Boss *> (it->source.get()); 
    if ( boss != 0 ) {
      // Attack the entire party in a "MultiAttack"
      if ( it->action == MULTIATTACK ) {	
	for ( coll_type::iterator itarget = pcs_.begin();
	      itarget != pcs_.end(); ++itarget ) {
	  boss->multiAttack( itarget->get() );
	}
	continue;
      }
      // If the target is dead, switch to the next in the list.
      if ( boss->getTarget() != 0 && boss->getTarget()->isDead() ) {
	bool bossGotTarget = setBossTarget( boss );
	if ( !bossGotTarget ) {
	  return false; 
	}
      }
    }

    if ( it->action ==  ATTACK ) {
      it->source->attack();
    } else if ( it->action ==  HEAL  ) {
      it->source->heal();
    } else if ( it->action ==  DEFEND ) {
      it->source->defend(); 
    } else {
      std::cout << "Incorrect action! Returning" << std::endl;
      return false; 
    }


    if ( !anyPCAlive() ) {
      std::cout << it->source->name() << " has vanquished your party." << std::endl;
      return false; 
    }

  }

  // Process the user actions
  for ( coll_type::iterator it = pcs_.begin(); it != pcs_.end(); ++it ) {
    

    if ( (*it)->isDead() ) {
      std::cout << (*it)->name() << " is dead" << std::endl;
      continue; 
    }


    std::cout << "Action for " << (*it)->name() << ": " << std::endl;
    QuickAction qa;
    std::string line;
    in >> line; 
    //std::getline( in, line );
    bool success = parseAction( line, qa ); 
    while ( !success ) {
      std::cout << "Invalid input, try again" << std::endl;
      in >> line; 
      success = parseAction( line, qa ); 
    }
    if ( qa.action ==  ATTACK ) {
      (*it)->attack();
    } else if ( qa.action ==  HEAL  ) {
      (*it)->heal();
    } else if ( qa.action ==  DEFEND ) {
      (*it)->defend(); 
    } else {
      std::cout << "Incorrect action! Returning" << std::endl;
      return false; 
    }


    if ( !anyNPCAlive() ) {
      std::cout << (*it)->name() << " has vanquished your foe." << std::endl;
      return false; 
    }

  }


  ++turn_; 

  return anyPCAlive() && anyNPCAlive(); 

}

// Set the next Boss target. It looks for the
// first character, in order, that is not dead. 
bool Battle::setBossTarget( Boss * boss ) {

  Entity const * entity = boss->getTarget();
 
  if ( boss == 0 ) {
    std::cout << "Boss is zero, cannot set new target" << std::endl;
    return false; 
  }
  for( coll_type::const_iterator pc = pcs_.begin(); pc != pcs_.end(); ++pc ) {
    if ( (*pc)->isAlive() ) {
      if ( pc->get() != entity  ){
	std::cout << boss->name() << " shifts their attacks to " << (*pc)->name() << std::endl;
      }
      boss->setTarget( pc->get() );
      return true;
    }
  }

  // Here there are no targets alive.
  return false; 

}

bool Battle::anyPCAlive() const {
  bool anyPCAliveRet = false; 
  for( coll_type::const_iterator pc = pcs_.begin(); pc != pcs_.end(); ++pc ) {
    if ( (*pc)->isAlive() ) anyPCAliveRet = true; 
  }
  return anyPCAliveRet;
}


bool Battle::anyNPCAlive() const {
  bool anyNPCAliveRet = false; 
  for( coll_type::const_iterator npc = npcs_.begin(); npc != npcs_.end(); ++npc ) {
    if ( (*npc)->isAlive() ) anyNPCAliveRet = true; 
  }
  return anyNPCAliveRet;
}


Battle::coll_type::iterator Battle::find_entity( std::string s )
{
  // Try to find the sources and targets: 
  coll_type::iterator source_pc = std::find_if( pcs_.begin(), pcs_.end(), MatchSource( s ) );
  coll_type::iterator source_npc = std::find_if( npcs_.begin(), npcs_.end(), MatchSource( s ) );

  if ( source_pc == pcs_.end() && source_npc == npcs_.end() ) {
    std::cout << "No matching source for " << s << std::endl;
    return pcs_.end(); 
  } else if ( source_pc == pcs_.end() ) {
    return source_npc;
  } else {
    return source_pc; 
  }
}


void Battle::printStats( std::ostream & out ) {
  out << "------------------------------- Turn : " << turn_ <<  "-------------------------------" << std::endl;
  out << "-----------------------------" << std::endl;
  out << "   === players:" << std::endl;
  // Remember! "coll_type" is just an alias for std::vector< std::shared_ptr<Entity> >
  for ( coll_type::const_iterator it = pcs_.begin();
	it != pcs_.end(); ++it ) {
    (*it)->printStats(out);
  }
  out << std::endl << "   === monsters:" << std::endl;
  for ( coll_type::const_iterator it = npcs_.begin();
	it != npcs_.end(); ++it ) {
    (*it)->printStats(out);
  }
  out << "-----------------------------" << std::endl;
}

void Battle::print( std::ostream & out ) {
  out << "------------------------------- Turn : " << turn_ <<  "-------------------------------" << std::endl;
  out << "--------------" << std::endl;
  out << "   === players:" << std::endl;
  // Remember! "coll_type" is just an alias for std::vector< std::shared_ptr<Entity> >
  for ( coll_type::const_iterator it = pcs_.begin();
	it != pcs_.end(); ++it ) {
    out << **it << std::endl;
  }
  out << std::endl << "   === monsters:" << std::endl;
  for ( coll_type::const_iterator it = npcs_.begin();
	it != npcs_.end(); ++it ) {
    out << **it << std::endl;
  }
  out << "--------------" << std::endl;
}

void Battle::printActions( std::ostream & out ) {

  out << " ----- Action list ----" << std::endl;
  for ( std::vector<QuickAction>::const_iterator it = actions_.begin();
	it != actions_.end(); ++it ) {
    if ( it->source == 0 ) {
      std::cout << "Configuration error." << std::endl;
      return;
    }
    out << it->source->name() << " will perform action " << it->action << " on their target : ";
    if ( it->source->getTarget() != 0) {
      out << it->source->getTarget()->name() << std::endl;
    } else {
      out << " NO TARGET!" << std::endl;
    }
  }

}


void Battle::printLog( std::ostream & out ) const
{

  out << "{\"Turns\":[" << std::endl;
  for ( unsigned int iturn = 0; iturn <= turn_; ++iturn ){

    out << "{\"Turn\":" << iturn << "," << std::endl;

    for ( auto i = npcs_.begin(); i != npcs_.end(); ++i ){
      (*i)->printActions(out, iturn);
      out << "," << std::endl;
    }
    for ( auto i = pcs_.begin(); i != pcs_.end(); ++i ){
      (*i)->printActions(out, iturn);
      // json does not like trailing commas
      if ( i != pcs_.end() - 1)
	out << "," << std::endl;
      else
	out << std::endl;
    }
    out << "}";
    if ( iturn != turn_ )
      out << "," << std::endl;
    else
      out << std::endl;
  }
  out << "]}" << std::endl;
}
