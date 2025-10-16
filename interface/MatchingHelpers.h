#ifndef MatchingHelpers_h
#define MatchingHelpers_h

#include "Character.h"
#include <string>

class MatchSource {
public:
  MatchSource(std::string s) : s_(s) {}

  bool operator()(std::shared_ptr<Character> const &character) {
    if (character->name() == s_) {
      return true;
    }
    return false;
  }

protected:
  std::string s_;
};

class MatchTarget {
public:
  MatchTarget(std::string s) : s_(s) {}
  bool operator()(std::shared_ptr<Character> const &character) {
    if (character->getTarget() != 0 && character->getTarget()->name() == s_) {
      return true;
    }
    return false;
  }

protected:
  std::string s_;
};

#endif
