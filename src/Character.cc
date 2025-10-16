#include "Character.h"
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <vector>

// Constructor
Character::Character(std::string class_name, std::string name,
                     unsigned int attack_power, unsigned int defense_power,
                     unsigned int heal_power, unsigned int max_mana, bool check)
    : class_name_(class_name), name_(name), hit_points_(100), max_hit_points_(100),
      mana_(max_mana), attack_power_(attack_power), defense_power_(defense_power),
      heal_power_(heal_power), target_(0) // uninitialized target
{
  if (check) {
    // Check to make sure your powers aren't outrageous
    checkPowers();
  }
}

// Destructor
Character::~Character() {}

void Character::input(std::string line) {
  std::vector<std::string> tokens;

  std::stringstream linestream(line);
  for (std::string each = ""; std::getline(linestream, each, ';');) {
    tokens.push_back(each);
  }
  if (tokens.size() >= 4) {
    name_ = tokens[0];
    attack_power_ = std::atoi(tokens[1].c_str());
    defense_power_ = std::atoi(tokens[2].c_str());
    heal_power_ = std::atoi(tokens[3].c_str());
  } else {
    std::cout << "Formatting error in input: unrecognized syntax in line : "
              << line << std::endl;
    return;
  }
  checkPowers();
}

// Input from "in"
void Character::input(std::istream &in) {
  std::string line;
  std::getline(in, line);
  input(line);
};

// Print to "out"

void Character::print(std::ostream &out) const {
  out << std::setw(12) << name_ << " (" << std::setw(10) << class_name_
      << "): HP=" << std::setw(5) << hit_points_ << ", mana = " << std::setw(5)
      << mana_;
  if (target_ != 0) {
    out << ", target=" << std::setw(12) << target_->name();
  } else {
    out << ", no target";
  }
}

void Character::printStats(std::ostream &out) const {
  out << std::setw(12) << name_ << " (" << std::setw(10) << class_name_
      << "): HP=" << std::setw(5) << hit_points_ << ", attack=" << std::setw(5)
      << attack_power_ << ", defend=" << std::setw(5) << defense_power_
      << ", heal=" << std::setw(5) << heal_power_ << ", mana = " << std::setw(5)
      << mana_;
  if (target_ != 0) {
    out << ", target=" << std::setw(12) << target_->name();
  } else {
    out << ", no target";
  }
}

// Reduce the hit points of "this" entity by "attack", mitigated by
// "defense_power"
int Character::reduceHitPoints(int attack) {
  int damage_taken = (attack - defense_power_);
  if (damage_taken < 0) {
    damage_taken = 0;
  } else if (damage_taken >= hit_points_) {
    // Protect against "overkill" in the stats accounting.
    damage_taken = hit_points_;
  }
  std::cout << name_ << " loses " << damage_taken << " hit points after attack "
            << attack << " and defense " << defense_power_ << std::endl;
  hit_points_ -= damage_taken;
  if (hit_points_ <= 0) {
    hit_points_ = 0;
    std::cout << name_ << ", the brave " << class_name_ << ", has died."
              << std::endl;
  }
  if (my_reduced_hit_points_.find(turn_) == my_reduced_hit_points_.end())
    my_reduced_hit_points_[turn_] = ActionVector();
  my_reduced_hit_points_[turn_].push_back(damage_taken);
  return damage_taken;
}

// Increase the hit points of "this" entity
int Character::increaseHitPoints(int heal) {
  int healed = heal; // Protect against "overheal" in the stats accounting.
  if (hitPoints() + healed >= max_hit_points_)
    healed = max_hit_points_ - hitPoints();
  hit_points_ += healed;
  if (my_increased_hit_points_.find(turn_) == my_increased_hit_points_.end())
    my_increased_hit_points_[turn_] = ActionVector();
  my_increased_hit_points_[turn_].push_back(healed);
  return healed;
}

// This will force the target of the other object to be "this" Character.
int Character::defaultDefend(Character *other) {
  if (other != 0) {
    setTarget(other);
  }
  if (getTarget() != 0) {

    if (getTarget()->isDead()) {
      std::cout << name_ << " : target " << getTarget()->name()
                << " is already dead." << std::endl;
      return 0;
    }
    std::cout << name_ << " defends against " << getTarget()->name()
              << " with defense mitigation " << defensePower() << std::endl;
    getTarget()->setTarget(this);
  } else {
    std::cout << name_ << " does not have a target to defend." << std::endl;
  }
  if (my_defends_.find(turn_) == my_defends_.end())
    my_defends_[turn_] = ActionVector();
  my_defends_[turn_].push_back(defensePower());
  return 0;
}

// In a heal, we increase the hit points
int Character::defaultHeal(Character *other) {
  if (mana_ < 10) {
    std::cout << name() << " does not have enough mana." << std::endl;
    return 0;
  }
  if (other != 0) {
    setTarget(other);
  }
  if (getTarget() != 0) {

    if (getTarget()->isDead()) {
      std::cout << name_ << " : target " << getTarget()->name()
                << " is already dead." << std::endl;
      my_heals_[turn_].push_back(0);
      return 0;
    }
    mana_ -= 10;
    auto healed = getTarget()->increaseHitPoints(heal_power_);
    std::cout << name() << " healed " << getTarget()->name()
              << " with heal power " << heal_power_ << " for " << healed
              << std::endl;
    if (my_heals_.find(turn_) == my_heals_.end())
      my_heals_[turn_] = ActionVector();
    my_heals_[turn_].push_back(healed);
    return healed;
  } else {
    std::cout << name_ << " does not have a target to heal." << std::endl;
    return 0;
  }
};

// In an attack, we reduce the hit points
int Character::defaultAttack(Character *other) {
  if (other != 0) {
    setTarget(other);
  }

  if (getTarget() != 0) {
    if (getTarget()->isDead()) {
      std::cout << name_ << " : target " << getTarget()->name()
                << " is already dead." << std::endl;
      my_attacks_[turn_].push_back(0);
      return 0;
    }

    int ap = attack_power_;
    auto attacked = getTarget()->reduceHitPoints(ap);
    std::cout << name() << " attacked " << getTarget()->name()
              << " with attack power " << ap << " for damage " << attacked
              << std::endl;
    if (my_attacks_.find(turn_) == my_attacks_.end())
      my_attacks_[turn_] = ActionVector();
    my_attacks_[turn_].push_back(attacked);
    return attacked;
  } else {
    std::cout << name_ << " does not have a target to attack." << std::endl;
    my_attacks_[turn_].push_back(0);
    return 0;
  }
};

// This will check to ensure the input values are sensible
bool Character::checkPowers() {

  is_magic_user_ = (mana_ > 0);
  if (attack_power_ + defense_power_ + heal_power_ > 20) {
    std::cout
        << name_
        << " : You cannot godmode here, your abilities can only sum to 20."
        << std::endl
        << "To punish you, the gods set your hitpoints to 1 and make you "
           "feeble as a kitten."
        << std::endl;
    hit_points_ = 1;
    attack_power_ = 0;
    defense_power_ = 0;
    heal_power_ = 0;
    return false;
  }
  if (attack_power_ < 0 || defense_power_ < 0 || heal_power_ < 0) {
    std::cout << name_ << " : Your powers cannot be negative." << std::endl
              << "To punish you, the gods set your hitpoints to 1 and make you "
                 "feeble as a kitten."
              << std::endl;
    hit_points_ = 1;
    attack_power_ = 0;
    defense_power_ = 0;
    heal_power_ = 0;
    return false;
  }

  return true;
};

// Some operators to support << and >>
std::ostream &operator<<(std::ostream &out, Character const &e) {
  e.print(out);
  return out;
}
std::istream &operator>>(std::istream &in, Character &e) {
  e.input(in);
  return in;
}

void Character::printActions(std::ostream &out, unsigned int iturn) const {

  auto all_actions = {std::make_pair("Attacks", &my_attacks_),
                     std::make_pair("Defends", &my_defends_),
                     std::make_pair("Heals", &my_heals_),
                     std::make_pair("DamageReceived", &my_reduced_hit_points_),
                     std::make_pair("HealingRecieved", &my_increased_hit_points_)};

  out << "\"" << name_ << "\":{";
  for (auto iaction = all_actions.begin(); iaction != all_actions.end();
       ++iaction) {
    auto actionname = iaction->first;
    auto actions = iaction->second;
    out << "\"" << actionname << "\":[";

    // Check if there are any actions for this turn
    auto p_action = actions->find(iturn);
    if (p_action != actions->end()) {
      auto action_vals = p_action->second;
      for (auto ival = action_vals.begin(); ival != action_vals.end(); ++ival) {
        out << *ival;
        // json does not like trailing comma
        if (ival + 1 != action_vals.end())
          out << ",";
      }
    } else {
      out << 0;
    }
    out << "]";

    if (iaction != all_actions.end() - 1) {
      out << ",";
    }
    out << std::endl;
  }
  out << "}";
}
