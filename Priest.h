#ifndef Priest_h
#define Priest_h

 /*  _____      _           _    */
 /* |  __ \    (_)         | |   */
 /* | |__) | __ _  ___  ___| |_  */
 /* |  ___/ '__| |/ _ \/ __| __| */
 /* | |   | |  | |  __/\__ \ |_  */
 /* |_|   |_|  |_|\___||___/\__| */

class Priest : public Entity{
 public :
    Priest( std::string name="", int healPower=0 ) :
    Entity( "Priest", name, 0, 0, healPower, 100) {
    };
  
   // In a heal, we increase the hit points
   virtual int heal( Entity * other=0 ) {
     return defaultHeal( other );
   };

 private :
};

#endif
