#include "Entity.h"
#include <iomanip> 
#include <vector>
#include <sstream>

// Constructor
Entity::Entity( std::string className,
		std::string name,
		unsigned int attackPower,
		unsigned int defensePower,
		unsigned int healPower,
		unsigned int maxMana
		):
  className_(className),
  name_(name),
  hitPoints_(100),
  mana_(maxMana),
  attackPower_(attackPower),
  defensePower_(defensePower),
  healPower_(healPower),
  target_(0) // uninitialized target
{
  // Check to make sure your powers aren't outrageous
  checkPowers();
}

// Destructor
Entity::~Entity(){}

// All of these default to "do nothing" and should be overriden in the derived classes. 
int Entity::attack( Entity * other ) { return 0;}
int Entity::heal  ( Entity * other ) { return 0;}
int Entity::defend( Entity * other ) { return 0;}

void Entity::input( std::string line ) 
{
  std::vector<std::string> tokens;

  std::stringstream linestream(line);	
  for (std::string each=""; std::getline(linestream, each, ';'); ){
    tokens.push_back(each);
  }
  if ( tokens.size() == 4 ) {
    name_ = tokens[0];
    attackPower_ = std::atoi( tokens[1].c_str() );
    defensePower_ = std::atoi( tokens[2].c_str()  );
    healPower_ = std::atoi( tokens[3].c_str()  );    
  } else {
    std::cout << "Formatting error in input: unrecognized syntax in line : " << line << std::endl;
    return; 
  }
  checkPowers();
}

// Input from "in"
void Entity::input( std::istream &in ) {
  std::string line;
  std::getline( in, line );
  input(line);
};

// Print to "out"

void Entity::print( std::ostream & out) const {
  out << std::setw(12)
      << name_ << " (" << std::setw(10) << className_ << "): HP=" << std::setw(5) << hitPoints_
      << ", mana = " << std::setw(5) << mana_;
  if ( target_ != 0 ) {
    out << ", target=" << std::setw(12) << target_->name();
  } else {
    out << ", no target";
  }
}



void Entity::printStats( std::ostream & out) const {
  out << std::setw(12)
      << name_ << " (" << std::setw(10) << className_ << "): HP=" << std::setw(5) << hitPoints_
      << ", attack=" << std::setw(5) << attackPower_
      << ", defend=" << std::setw(5) << defensePower_
      << ", heal="   << std::setw(5) << healPower_
      << ", mana = " << std::setw(5) << mana_;
  if ( target_ != 0 ) {
    out << ", target=" << std::setw(12) << target_->name();
  } else {
    out << ", no target";
  }
}



// Reduce the hit points of "this" entity by "attack", mitigated by "defensePower"
int Entity::reduceHitPoints( int attack ) {  
  int diff = (attack - this->defensePower_);
  if ( diff < 0 )
    diff = 0;
  std::cout << name_ << " loses " << diff << " hit points after attack " << attack << " and defense " << this->defensePower_ << std::endl;
  this->hitPoints_ -= diff;
  if (this->hitPoints_ <= 0) {
    this->hitPoints_ = 0; 
    std::cout << name_ << ", the brave " << className_ << ", has died." << std::endl;
  }
  return this->hitPoints_; 
}

// Increase the hit points of "this" entity
int Entity::increaseHitPoints( int heal ) { 
  this->hitPoints_ += heal; 
  if (this->hitPoints_ > 100 ) {
    this->hitPoints_ = 100; 
  }
  return this->hitPoints_; 
}


// This will force the target of the other object to be "this" Entity.
int Entity::defaultDefend( Entity * other ) {
  if ( other != 0 ) {
    setTarget(other);
  }
  if ( getTarget() != 0 ) {

    if ( getTarget()->isDead() ) {
      std::cout << name_ << " : target " << getTarget()->name() << " is already dead." << std::endl;
      return 0;
    }
    std::cout << name_ << " defends against " << getTarget()->name() << ", defense mitigation " << defensePower() << std::endl;
    getTarget()->setTarget(this);
  } else {
    std::cout << name_ << " does not have a target to defend." << std::endl;
  }
  return 0;     
}


// In a heal, we increase the hit points
int Entity::defaultHeal( Entity * other ) {
  if ( mana_ < 10 ) {
    std::cout << name() << " does not have enough mana." << std::endl;
    return 0;
  }
  if ( other != 0 ) {
    setTarget(other);
  }  
  if ( getTarget() != 0 ){

    if ( getTarget()->isDead() ) {
      std::cout << name_ << " : target " << getTarget()->name() << " is already dead." << std::endl;
      return 0;
    }

    std::cout << name() << " heals " << getTarget()->name() << " for " << healPower() << std::endl;
    mana_ -= 10;
    return getTarget()->increaseHitPoints( this->healPower_ );
  }
  else {
    std::cout << name_ << " does not have a target to heal." << std::endl;
    return 0;
  }
};


// In an attack, we reduce the hit points
int Entity::defaultAttack( Entity * other ) {
  if ( other != 0 ) {
    setTarget(other);
  }

  if ( getTarget() != 0 ) {
    if ( getTarget()->isDead() ) {
      std::cout << name_ << " : target " << getTarget()->name() << " is already dead." << std::endl;
      return 0;
    }

    int ap = this->attackPower_;
    std::cout << name() << " attacks " << getTarget()->name() << " with attack power " << ap << std::endl;
    return getTarget()->reduceHitPoints( ap );
  } else {
    std::cout << name_ << " does not have a target to attack." << std::endl;
    return 0;
  }
};

// This will check to ensure the input values are sensible
bool Entity::checkPowers() {

  isMagicUser_ = ( mana_ > 0);
  if ( attackPower_ + defensePower_ + healPower_ > 20 ) {
    std::cout << "You cannot godmode here, your abilities can only sum to 20." << std::endl
	      << "To punish you, the gods set your hitpoints to 1 and make you feeble as a kitten." << std::endl;
    hitPoints_ = 1;
    attackPower_ = 0;
    defensePower_ = 0;
    healPower_ = 0;
    return false; 
  }
  if ( attackPower_ < 0 || defensePower_ < 0 || healPower_ < 0 ) {
    std::cout << "Your powers cannot be negative." << std::endl
	      << "To punish you, the gods set your hitpoints to 1 and make you feeble as a kitten." << std::endl;
    hitPoints_ = 1;
    attackPower_ = 0;
    defensePower_ = 0;
    healPower_ = 0;
    return false;
  }

  return true; 
};

// Some operators to support << and >>
std::ostream & operator<<( std::ostream & out, Entity const & e){ e.print(out); return out; }
std::istream & operator>>( std::istream & in, Entity & e) {e.input(in); return in;}
