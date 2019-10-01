#ifndef Battle_h
#define Battle_h


// --------------------------------------
 /*  ____        _   _   _       */
 /* |  _ \      | | | | | |      */
 /* | |_) | __ _| |_| |_| | ___  */
 /* |  _ < / _` | __| __| |/ _ \ */
 /* | |_) | (_| | |_| |_| |  __/ */
 /* |____/ \__,_|\__|\__|_|\___| */
// --------------------------------------
// This is the main driver of the game.
//
// It reads in two configuration files
// consisting of the player characters (PCs)
// and the non-player characters (NPCs) in order
// of attack. The battle will they play out until
// one of the parties is empty. 
//
// The configuration files will all support comments in the form of an exclamation point ("!").
// The configuration files will have fields separated by a semicolon (";")
//
// The PC configuration syntax is as follows (omit the < >, they are there for notation here only)
// <name>;<type>;<attack power>;<defense power>;<heal power>;
//   <name> :         Name of the character as a string, like "Moe"
//   <type> :         Type of character as a string, like "Warrior"
//   <attack power>:  Attack power as an integer, like "20"
//   <defense power>: Defense power as an integer, like "20"
//   <heal power>:    Heal power as an integer, like "20"
// Example:
// Curly;Rogue;0;20;0;
// Moe;Warrior;20;0;0;
// Larry;Priest;0;0;20;
// 
//
// The NPC configuration is similar, but may have different attributes since
// they have a "multi-attack power" which attacks everyone in the party.
//  <name> <type> <attack power> <defense power> <heal power> <multi attack power>
// Example: 
// Shemp;Boss;20;0;0;5;
//
//
// There is then a syntax for the battle itself, called a "script". It will consist of a set of
// actions your team will apply per turn. 
// <name>;<action>;<target> 
//    <name>  :   Name of character performing action
//    <action>:   Name of the action to apply (like "attack", "heal", "defend")
//    <target>:   Name of the target on which to perform the action
//
// Example:
//  Curly;attack;Shemp;          <<--- Curly attacks Shemp
//  Moe;defend;Shemp;            <<--- Moe switches Shemp's target to be Moe, defends.
//  Larry;heal;Moe;              <<--- Larry heals moe
//
// There is also a command-line interface that will cycle through your players and ask for
// actions prompted from "std::cin" like:
// Action for Curly? 
//   (You can then type "attack;Shemp;")
// Action for Moe? 
//   (You can then type "defend;Shemp;")
// Action for Larry?
//   (You can then type "heal;Moe;")
//
//
//
//
//
//
// The Boss "script" will also be similarly defined, but it will contain a description and 
// the boss will attack whoever the target is.
// The target of the Boss is initially set to the first person in your configuration file. 
// For instance, Shemp may attack the "first" person on your list (above, this is Moe), and then
// attack everyone on your list. This will happen once per turn. Even in the command-line interface,
// the same sequence is executed once per turn. If the "defend" method is called, however,
// the target of 
//
// Shemp;attack;target           <<---- Shemp will attack the first person on your list
// Shemp;attack;all              <<---- Shemp will attack your entire party with "multi_attack"
// 
// Suppose that your Warrior Moe casts "defend" against Shemp, then Shemp's target will be
// Moe. 
//
// --------------------------------------


#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <algorithm>
#include "Entity.h"
#include "Boss.h"
#include "Warrior.h"
#include "Rogue.h"
#include "Priest.h"

class Battle {
 public:



  // Create a "typedef" that sets an alias of one type to another to save typing.
  typedef std::vector< std::shared_ptr<Entity> > coll_type;

 Battle(): turn_(0), description_(""), userTurns_(false), scripted_(false) {}
  ~Battle() {}

  // Read the configuration for this Battle.
  // It will consist of the player characters, the non-player characters,
  // in their order of attack. 
  bool readPCConfiguration ( std::string filename );
  bool readNPCConfiguration( std::string filename );

  // Read a script of actions. It will consist of a
  // list of actions per turn to execute.
  // This will append to the existing actions. 
  bool loadActionScript (std::string filename);
  // Clear the actions
  void clearActions() { actions_.clear(); } 

  // Are we letting the user work in turns? 
  void setUserTurns(bool userTurns=true) { userTurns_  = userTurns;}
  bool userTurns() const { return userTurns_;}

  // Perform scripted actions
  bool performScriptedActions();

  // For user turn-by-turn action
  bool performUserActions( std::istream & in = std::cin );


  // Print this turn
  virtual void print( std::ostream & out = std::cout );
  // Print all of the stats of the players in the battle
  void printStats( std::ostream & out = std::cout );
  // Print the actions queued
  void printActions( std::ostream & out = std::cout );

  // Return the description
  std::string description() const { return description_; }


  // Print the splash screen
  void splash( std::ostream & out = std::cout ) const;

  // Print the logfile of the battle
  void printLog( std::ostream & out) const; 
  
 private:


  unsigned int turn_;        // What turn is it? 
  std::string  description_; // Fun description of battle. 
  bool userTurns_;           // Input turn-by-turn user input? By default, no.
  coll_type pcs_;            // Player characters
  coll_type npcs_;           // Non-player characters

  bool      scripted_;       // Is this fight scripted or user-turns? 


  //
  //
  // Internal workings of "Battle":
  //    Each turn, Entities will perform actions on each other. 
  //    However, we don't want to have to parse the text file
  //    over and over again, so we store the actions in a vector. 
  //
  // 

  // Find a source named "s"
  coll_type::iterator find_entity( std::string s ); 


  // This will enumerate the possible actions that can be taken
  enum ActionType {
    ATTACK=0,HEAL,DEFEND,MULTIATTACK,N_ACTIONS
  };

  // A private "struct" (i.e. a class with all public members) to store the source and action type
  struct QuickAction{ 
    std::shared_ptr<Entity> source;
    ActionType action; 
    QuickAction(std::shared_ptr<Entity> s=0, ActionType a=N_ACTIONS) : source(s), action(a) {}
  };
  std::vector<QuickAction>   actions_;  // List of scripted actions to perform per turn.


  // Add an action
  // in the format "source;action;target;"
  bool parseAction( std::string, QuickAction & qa );
  
  bool anyNPCAlive() const;
  bool anyPCAlive() const;

  // Check if the Boss's target is dead. If so, set to the first living member
  // of the party. Otherwise, return false; 
  bool setBossTarget( Boss * boss );

};


#endif
