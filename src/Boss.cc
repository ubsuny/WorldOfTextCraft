#include "Boss.h"
#include <iomanip>
#include <sstream>

Boss::Boss(std::string name, int attack_power, int defense_power, int heal_power,
           int mana, int multi_attack_power)
    : Character("Boss", name, attack_power, defense_power, heal_power, mana, false) {
  multi_attack_power_ = multi_attack_power;
  hit_points_ = 500;
  max_hit_points_ = 500;
};

int Boss::multiAttack(Character *other) {

  Character *originalTarget = getTarget();

  if (other != 0) {
    setTarget(other);
  }
  int ap = this->multi_attack_power_;

  if (getTarget() != 0) {
    std::cout << name() << " multi-attacks " << getTarget()->name()
              << " with attack power " << ap << std::endl;
    int retval = getTarget()->reduceHitPoints(ap);
    setTarget(originalTarget);
    if (my_attacks_.find(turn_) == my_attacks_.end())
      my_attacks_[turn_] = ActionVector();
    my_attacks_[turn_].push_back(retval);
    return retval;
  } else {
    std::cout << name_ << " does not have a target to attack." << std::endl;
    setTarget(originalTarget);
    return 0;
  }
};

// Print to "out"
void Boss::printStats(std::ostream &out) const {
  out << std::setw(12) << name_ 
    << " (" << std::setw(10) << class_name_ 
    << "): HP=" << std::setw(5) << hit_points_ 
    << ", attack=" << std::setw(5) << attack_power_ 
    << ", defend=" << std::setw(5) << defense_power_
    << ", heal=" << std::setw(5) << heal_power_;
    
  if (isMagicUser()) {
    out << ", mana = " << std::setw(5) << mana_;
  }
  out << ", multi =" << std::setw(5) << multi_attack_power_;
  if (target_ != 0) {
    out << ", target=" << std::setw(12) << target_->name();
  } else {
    out << ", no target";
  }
}

// Print to "out"
void Boss::print(std::ostream &out) const {
  out << std::setw(12) << name_ << " (" << std::setw(10) << class_name_
      << "): HP=" << std::setw(5) << hit_points_ << ", mana = " << std::setw(5)
      << mana_;
  if (target_ != 0) {
    out << ", target=" << std::setw(12) << target_->name();
  } else {
    out << ", no target";
  }
}

void Boss::input(std::string line) {
  std::vector<std::string> tokens;

  std::stringstream linestream(line);
  for (std::string each = ""; std::getline(linestream, each, ';');) {
    tokens.push_back(each);
  }
  if (tokens.size() >= 6) {
    name_ = tokens[0];
    attack_power_ = std::atoi(tokens[1].c_str());
    defense_power_ = std::atoi(tokens[2].c_str());
    heal_power_ = std::atoi(tokens[3].c_str());
    mana_ = std::atoi(tokens[4].c_str());
    multi_attack_power_ = std::atoi(tokens[5].c_str());

    std::cout << "Input boss: " << *this << std::endl;
  } else {
    std::cout << "Formatting error in input: unrecognized syntax in line : "
              << line << std::endl;
    return;
  }
}
