# WorldOfTextCraft
This is a trivial turn-based role-playing fighting game, designed to teach C++.

Apologies to Blizzard Entertainment and the Three Stooges for cribbing (and butchering) their content. 

The goal was to make this "bare bones" without a complicated build so people can 
see for themselves what is involved in the software. This does require C++11
because of the use of a `vector< shared_ptr<T> >` for memory management. There is a very trivial Makefile
to create three executables: 

<ul>
<li> <b>test_entity</b>: test the base fighting class functionality </li>
<li> <b>test_boss</b>: test the boss functionality </li>
<li> <b>test_battle</b>: test a pre-scripted battle</li>
<li> <b>WorldOfTextCraft</b>: the actual text-based RPG. </li> 
</ul>


In order to compile just type "make". 


## PHY410/505 at UB

### SENS
For PHY410/505 students working on the SENS cluster at UB:

```
> use devtoolset-2
> git clone git@github.com:rappoccio/WorldOfTextCraft.git
> cd WorldOfTextCraft
> make
```

### VIDIA 

```
> git clone git@github.com:rappoccio/WorldOfTextCraft.git
> cd WorldOfTextCraft
> make
```

### Docker image

If you are using the [CompPhys](https://github.com/rappoccio/CompPhys)
package and the
[docker image](https://hub.docker.com/r/srappoccio/compphys) you
should add this to the `results` folder **in your host OS**:

```
> cd /your/working/directory/results
> git clone git@github.com:rappoccio/WorldOfTextCraft.git
```

Then go to your `CompPhys` folder:

```
> cd /your/working/directory/CompPhys
> ./runDocker.sh srappoccio/compphys:latest 1
```

This will launch the docker image, so you will be in the `/results`
folder in docker. You can then:


```
> cd WorldOfTextCraft
> make
```



## Usage

```
> ./WorldOfTextCraft player_config boss_config boss_script
```

Here, we have:
- player_config: player configuration text file
- boss_config : boss configuration text file
- boss_script : the script of moves the boss will execute



For instance:

```
./WorldOfTextCraft AshenVerdict.txt Arthas.txt ArthasAttacks.txt
```

## Syntax

In all syntax files, lines starting with "!" will be ignored (i.e. comments)

- Player configuration 
  - One line per party member, syntax : `type;name;attack_power;defense_power;heal_power;`
  - Example: 
  ```
  !
  ! Test party configuration: 
  ! Warrior, Rogue, Priest
  ! Fields are : 
  ! name, entity type, attack power, heal power, defense power
  Rogue;Curly;20;0;0;
  Priest;Larry;0;0;8;
  Warrior;Moe;0;3;0;
  ```
- Boss configuration : 
  - First line is the description of the battle to be displayed. 
  - Then one line per boss, syntax: `name;attack_power;heal_power;defense_power;mana;multi_attack`
  - Example: 
  ```
  !Description:
  Larry, Moe, and Curly are the Three Stooges. Their poor friend Shemp has been bitten by a zombie and is trying to kill them. Help Larry, Moe, and Curly put Shemp out of his misery. Nyuk Nyuk Nyuk.
  ! Non-Player character. Fields are "name", attack power, heal power, defense power, mana, multi attack power
  Shemp;20;0;0;10;5;
  ```
- Boss script: 
  - One line per action, syntax: `name;action;target_name;`
  - Here, "action" can be "attack", "heal", or "defend". 
  - "target_name" can be "target" (to target the first of the Player's party) or "all" to attack the entire party at once.
  - Example:
  ```
  ! Test battle configuration.
  Shemp;attack;target;
  Shemp;attack;all;
  ```
- Player script: (for `test_battle` only, not for `WorldOfTextCraft`): 
  - One line per action, syntax: `name;action;target_name`
  - Here, "action" can be "attack", "heal", or "defend"
  - "target_name" must refer to a member of the Boss party. 
  - Example:
  ```
  ! Test battle configuration.
  Curly;attack;Shemp;
  Moe;defend;Shemp;
  Larry;heal;Moe;
  ```
  
  
## Game Mechanics

The game is rather simple. Initially there are three player classes, all with 100 hit points,
and one Boss class, with 500 hit points.
Once a character reaches 0 hit points, it dies. All characters have a common base C++ class
"Entity", and then override the functionality in that base C++ class to be player-class-specific: 
- Rogue: can cast "attack", which reduces the hit points of a target by 20.
- Warrior: can cast "defend", which forces the target to attack the Warrior. They have a defense mitigation modifier ("defense power") that reduces the hit points lost upon an attack. The final damage is "attack_power - defense_power". 
- Priest: can cast "heal", which increases the hit points of a target by 20. Starts with 100 mana. 
- Boss: can cast any spell, and also has a special "multi_attack", where it attacks the entire party with another stat "multi_attack_power". This should be less than the attack power. 

You can adjust the game mechanics to add player classes of your own! 

## Playing WorldOfTextCraft : The Lich King Scenario

To execute "The Lich King" Scenario, execute: 

```
> ./WorldOfTextCraft AshenVerdict.txt Arthas.txt ArthasAttacks.txt 
```

You will be prompted as follows: 

```
reading PC configuration
Added entity:     Fordring (   Warrior): HP=  100, mana =     0, no target
Added entity:       Thrall (    Priest): HP=  100, mana =   100, no target
Added entity:     Mograine (     Rogue): HP=  100, mana =     0, no target
reading NPC configuration
Input boss:       Arthas (      Boss): HP=  500, mana =     0, no target
reading NPC action script
|--------------------------------------------|
|--------------------------------------------|
|--------------------------------------------|
|--------------------------------------------|
|        __    __           _     _         |
|       / / /\ \ \___  _ __| | __| |        |
|       \ \/  \/ / _ \| '__| |/ _` |        |
|        \  /\  / (_) | |  | | (_| |        |
|         \/  \/ \___/|_|  |_|\__,_|        |
|                                           |
|                       __                  |
|                 ___  / _|                 |
|                / _ \| |_                  |
|               | (_) |  _|                 |
|                \___/|_|                   |
|                                           |
| _____          _     ___           __ _   |
|/__   \_____  _| |_  / __\ __ __ _ / _| |_ |
|  / /\/ _ \ \/ / __|/ / | '__/ _` | |_| __||
| / / |  __/>  <| |_/ /__| | | (_| |  _| |_ |
| \/   \___/_/\_\\__\____/_|  \__,_|_|  \__||
|                                           |
|--------------------------------------------|
|--------------------------------------------|
|--------------------------------------------|
Welcome brave traveler...
You venture forth from your stronghold in Azeroth to the icy continent of Northrend, accompanied only by your fellow adventurers and your courage. You have tracked Arthas Menethil to his Frozen Throne in the plagued wastelands of Icecrown. Once a brave champion of Lordaeron, Arthas was corrupted by evil and his soul subsumed by the shaman Ner'zhul upon taking the cursed Runeblade, Frostmourne, forming the entity known as the Lich King, threatening all life in Azeroth to become mindless undead slaves of the Burning Legion. The Knights of the Ebon Blade and the Argent Crusade have now formed the Ashen Verdict, and tasked you to destroy the Lich King and end his undead plague upon Azeroth. As you reach the Frozen Throne, the Lich King's seat, you hear a voice in your head. The Lich King whispers "Young heroes, I was once like you. You have come to this place seeking to bring judgement upon the damned. But, be warned. In the end, all that awaits you is death. Only then will you understand, you've been following in my footsteps all along. So come then, you heroes! Come in all your power and glory! For in this final hour, all must serve the one... true... king... Frostmourne HUNGERS....". The battle is joined. 
Are you ready to begin? [Y/n] 
```

You can type "n" to exit, or "y" to begin. You will then see a description of the Boss's first attack: 

```
Excellent... let us begin...
 ----- Action list ----
Arthas will perform action 0 on their target :  NO TARGET!
Arthas will perform action 3 on their target :  NO TARGET!
------------------------------- Turn : 0-------------------------------
--------------
   === players:
    Fordring (   Warrior): HP=  100, mana =     0, no target
      Thrall (    Priest): HP=  100, mana =   100, no target
    Mograine (     Rogue): HP=  100, mana =     0, no target

   === monsters:
      Arthas (      Boss): HP=  500, mana =     0, no target
--------------
Arthas attacks Fordring with attack power 20
Fordring loses 10 hit points after attack 20 and defense 10
Arthas multi-attacks Fordring with attack power 8
Fordring loses 0 hit points after attack 8 and defense 10
Arthas multi-attacks Thrall with attack power 8
Thrall loses 8 hit points after attack 8 and defense 0
Arthas multi-attacks Mograine with attack power 8
Mograine loses 8 hit points after attack 8 and defense 0
Action for Fordring: 
```

You are now at the action prompt. The syntax to perform actions is the same as for the scripting language, so type:

`Fordring;defend;Arthas`

The game will then print the status of your attack:

`Fordring defends against Arthas, defense mitigation 10`



You will then have the next player. Enter the next action, and you will again see a printout of the status of your attack:
```
Action for Thrall: 
Thrall;heal;Mograine;
Thrall heals Mograine for 12
```

Finally you will see your next character. Enter the next action, and another printout will occur:

```
Mograine;attack;Arthas;
Mograine attacks Arthas with attack power 20
Arthas loses 20 hit points after attack 20 and defense 0
```


This reaches the end of your turn, so the status is printed again, and it is the boss's turn once more:

```
------------------------------- Turn : 1-------------------------------
--------------
   === players:
    Fordring (   Warrior): HP=   90, mana =     0, target=      Arthas
      Thrall (    Priest): HP=   92, mana =    90, target=    Mograine
    Mograine (     Rogue): HP=  100, mana =     0, target=      Arthas

   === monsters:
      Arthas (      Boss): HP=  480, mana =     0, target=    Fordring
--------------
Arthas attacks Fordring with attack power 20
Fordring loses 10 hit points after attack 20 and defense 10
Arthas multi-attacks Fordring with attack power 8
Fordring loses 0 hit points after attack 8 and defense 10
Arthas multi-attacks Thrall with attack power 8
Thrall loses 8 hit points after attack 8 and defense 0
Arthas multi-attacks Mograine with attack power 8
Mograine loses 8 hit points after attack 8 and defense 0
Action for Fordring: 
```

You can then cycle through until your party is victorious (the Boss dies), or fails (all of you die). Then the scenario ends. To exit prematurely, simply "control-C" out. 

## Battle Log

The C++ game will create a log of actions in a
[JSON](https://www.json.org) format. This will look something like:

```
    {"Turns":[                         # Name of the entire structure ("Turns")
    {"Turn":0,                         # This turn number (0)
    "Arthas":{"Attacks":[10,0,8,8],    # "Name of attacker":{ "action":[values for action],...}
    "Defends":[0],
    "Heals":[0],
    "DamageReceived":[20],
    "HealingRecieved":[0]
    },
    "Fordring":{"Attacks":[0],         # "Name of next attacker":{ "action":[values for action],...}
    "Defends":[10],
    "Heals":[0],
    "DamageReceived":[10,0],
    "HealingRecieved":[0]
    },
    "Thrall":{"Attacks":[0],           # and so on
    "Defends":[0],
    "Heals":[8],
    "DamageReceived":[8],
    "HealingRecieved":[0]
    },
    "Mograine":{"Attacks":[20],
    "Defends":[0],
    "Heals":[0],
    "DamageReceived":[8],
    "HealingRecieved":[8]
    }
    },
    {"Turn":1,                       # Next turn, and so on. 
    ...
    }
    }
```

There is a script `readbattle.py` that will parse this JSON file and
put the data into a `numpy ndarray`. If you set the `--verbose` flag
on the command line, it will also print a table that looks more
columnar like this, with the column headings printed on top: 

```
['Attacks', 'Defends', 'Heals', 'DamageReceived', 'HealingRecieved']
------------ turn :  0  ---------------
====     Arthas : 26,  0,  0, 20,  0,
====   Fordring :  0, 10,  0, 10,  0,
====     Thrall :  0,  0,  8,  8,  0,
====   Mograine : 20,  0,  0,  8,  8,
------------ turn :  1  ---------------
====     Arthas : 26,  0,  0, 20,  0,
====   Fordring :  0, 10,  0, 10,  0,
====     Thrall :  0,  0,  8,  8,  0,
====   Mograine : 20,  0,  0,  8,  8,
...

```

The `numpy` array looks like:

```
[[[26.  0.  0. 20.  0.]             <--- Character 0 (in this case, Arthas)
  [26.  0.  0. 20.  0.]
  [26.  0.  0. 20.  0.]
  [26.  0.  0. 20.  0.]
  [26.  0.  0. 20.  0.]
  [26.  0.  0. 20.  0.]
  [26.  0.  0. 20.  0.]
  [26.  0.  0. 20.  0.]
  [26.  0.  0. 20.  0.]
  [26.  0.  0. 20.  0.]
  [28.  0.  0. 20.  0.]
  [28.  0.  0. 20.  0.]
  [28.  0.  0. 20.  0.]
  [28.  0.  0. 20.  0.]]

 [[ 0. 10.  0. 10.  0.]            <--- Character 1 (in this case, Fordring)
  [ 0. 10.  0. 10.  0.]
  [ 0. 10.  0. 10.  0.]
  [ 0. 10.  0. 10.  0.]
  [ 0. 10.  0. 10.  0.]
  [ 0. 10.  0. 10.  0.]
  [ 0. 10.  0. 10.  0.]
  [ 0. 10.  0. 10.  0.]
  [ 0. 10.  0. 10.  0.]
  [ 0.  0.  0. 10.  0.]
  [ 0.  0.  0.  0.  0.]
  [ 0.  0.  0.  0.  0.]
  [ 0.  0.  0.  0.  0.]
  [ 0.  0.  0.  0.  0.]]

...
```

The output array is called `npdata` and can be used for various
analysis purposes. 
