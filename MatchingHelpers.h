#ifndef MatchingHelpers_h
#define MatchingHelpers_h

#include "Entity.h"
#include <string>

class MatchSource {
public : 
  MatchSource( std::string s ) : s_(s) {}

  bool operator() ( std::shared_ptr<Entity> const & entity ) {
    if ( entity->name() == s_ ) {
      return true; 
    }
    return false; 
  }

protected : 
  std::string s_; 
};


class MatchTarget {
public : 
  MatchTarget( std::string s ) : s_(s) {}
  bool operator() ( std::shared_ptr<Entity> const & entity ) {
    if ( entity->getTarget() != 0 && entity->getTarget()->name() == s_ ) {
      return true; 
    }
    return false; 
  }
protected : 
  std::string s_; 
};



#endif
