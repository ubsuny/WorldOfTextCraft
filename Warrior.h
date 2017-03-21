#ifndef Warrior_h
#define Warrior_h


 /* __          __             _             */
 /* \ \        / /            (_)            */
 /*  \ \  /\  / /_ _ _ __ _ __ _  ___  _ __  */
 /*   \ \/  \/ / _` | '__| '__| |/ _ \| '__| */
 /*    \  /\  / (_| | |  | |  | | (_) | |    */
 /*     \/  \/ \__,_|_|  |_|  |_|\___/|_|    */
                                         

class Warrior : public Entity{
 public :
   Warrior( std::string name="", int defensePower=0 ) :
    Entity( "Warrior", name, 0, defensePower, 0) {
    };

   // This will force the target of the other object to be "this" Entity.
   virtual int defend( Entity * other =0) {
     return defaultDefend(other);
   }

};

#endif
