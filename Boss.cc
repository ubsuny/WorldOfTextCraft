#include "Boss.h"
#include <sstream>
#include <iomanip>

Boss::Boss( std::string name, int attackPower, int defensePower, int healPower, int mana, int multiAttackPower ) :
  Entity( "Boss", name, attackPower, defensePower, healPower, mana, false) {
  multiAttackPower_ = multiAttackPower; 
  hitPoints_ = 500;
  maxHitPoints_ = 500;
};


   
int Boss::multiAttack( Entity * other ) {

  Entity * originalTarget = getTarget();

  if ( other != 0 ) {
    setTarget(other);
  }
  int ap = this->multiAttackPower_;

  if ( getTarget() != 0 ) {
    std::cout << name() << " multi-attacks " << getTarget()->name() << " with attack power " << ap<< std::endl;
    int retval = getTarget()->reduceHitPoints( ap );
    setTarget(originalTarget);
    if ( myAttacks_.find(turn_) == myAttacks_.end() )
      myAttacks_[turn_] = action_vector(); 
    myAttacks_[turn_].push_back(retval);
    return retval;
  } else {
    std::cout << name_ << " does not have a target to attack." << std::endl;
    setTarget( originalTarget);
    return 0;
  }
};
   

// Print to "out"
void Boss::printStats( std::ostream & out) const {
  out << std::setw(12)
      << name_ << " (" << std::setw(10) << className_ << "): HP=" << std::setw(5) << hitPoints_
      << ", attack=" << std::setw(5) << attackPower_
      << ", defend=" << std::setw(5) << defensePower_
      << ", heal="   << std::setw(5) << healPower_;
  if ( isMagicUser() ) {
    out << ", mana = " << std::setw(5) << mana_;
  }
  out << ", multi =" << std::setw(5) << multiAttackPower_;
  if ( target_ != 0 ) {
    out << ", target=" << std::setw(12) << target_->name();
  } else {
    out << ", no target";
  }
}


// Print to "out"
void Boss::print( std::ostream & out) const {
  out << std::setw(12)
      << name_ << " (" << std::setw(10) << className_ << "): HP=" << std::setw(5) << hitPoints_
      << ", mana = " << std::setw(5) << mana_;
  if ( target_ != 0 ) {
    out << ", target=" << std::setw(12) << target_->name();
  } else {
    out << ", no target";
  }
}




void Boss::input( std::string line ) 
{
  std::vector<std::string> tokens;

  std::stringstream linestream(line);	
  for (std::string each=""; std::getline(linestream, each, ';'); ){
    tokens.push_back(each);
  }
  if ( tokens.size() == 6 ) {
    name_ = tokens[0];
    attackPower_ = std::atoi( tokens[1].c_str() );
    defensePower_ = std::atoi( tokens[2].c_str() );
    healPower_ = std::atoi( tokens[3].c_str() );    
    mana_ = std::atoi( tokens[4].c_str() );
    multiAttackPower_ = std::atoi( tokens[5].c_str() );

    std::cout << "Input boss: " << *this << std::endl;
  } else {
    std::cout << "Formatting error in input: unrecognized syntax in line : " << line << std::endl;
    return; 
  }

}
