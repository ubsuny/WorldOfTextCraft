#ifndef Rogue_h
#define Rogue_h


 /*  _____                         */
 /* |  __ \                        */
 /* | |__) |___   __ _ _   _  ___  */
 /* |  _  // _ \ / _` | | | |/ _ \ */
 /* | | \ \ (_) | (_| | |_| |  __/ */
 /* |_|  \_\___/ \__, |\__,_|\___| */
 /*               __/ |            */
 /*              |___/             */

class Rogue : public Entity{
 public :
   Rogue( std::string name="", int attackPower=0 ) :
    Entity( "Rogue", name, attackPower, 0, 0) {
    };
  
   // In an attack, we reduce the hit points
   virtual int attack( Entity * other=0 ) {
     return defaultAttack(other);
   };
};

#endif
