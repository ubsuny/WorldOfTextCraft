import json
import numpy as np
import argparse

def readbattle(infile, verbose=False):
    '''
    Description: script to read the battle log for WorldOfTextCraft. 
    The battle log is in JSON format, and this script prints
    out a nicely-formatted table, and creates a numpy array called "npdata"
    for analysis. 


    The data is input in JSON format.
    The file will look something like this:


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
    '''
    
    with open(infile) as json_file:
        data = json.load(json_file)

    # The total data structure is called "turns".
    turns = data["Turns"]
    # Get the list of character names from the json file: 
    charnames = list(turns[0].keys())
    # The json file for each turn includes the "Turn" entry to indicate the
    # turn number, so remove that. 
    charnames.remove("Turn")
    # Now we get a list of the action headers (like "Attacks", "Defends", "Heals", etc)
    actionheaders = [ s for s in turns[0][charnames[0]] ]

    if verbose:
        print("Action headers:")
        print(actionheaders)

    # Get the number of characters, turns, and actions
    nchars = len(charnames)
    nturns = len( data["Turns"] )
    nactiontypes = len(actionheaders)

    # Initialize the data. It will have dimensions:
    # (number of characters, number of turns, number of actions)
    # 
    npdata = np.zeros( (nchars,nturns,nactiontypes) )

    # Now we deconstruct the json file. 
    # Loop over turns. 
    for iturn,turn in enumerate( turns ):
        turnval = turn["Turn"]
        if verbose: 
            print ("------------ turn : ", turnval, " ---------------")
        # Loop over characters in this turn
        for icharname, charname in enumerate(charnames):
            s = "==== %10s :" % (charname)
            vals = turn[charname]
            # Loop over actions for this character
            for iact,val in vals.items():
                s += "%3d," % (sum(val) )        
                npdata[icharname,turnval,actionheaders.index(iact)] = sum(val)
            if verbose: 
                print(s)

    # We finally have the numpy data. 
    return npdata


def main():

    # Get the input file
    parser = argparse.ArgumentParser(description='Parse battle logs')
    parser.add_argument('--infile',action='store',
                        default='battlelog.txt',
                        dest='infile')
    parser.add_argument('--verbose', '-v', action='store_true',
                        default=False,
                        dest='verbose')
    args = parser.parse_args()
    

    npdata = readbattle(infile=args.infile, verbose=args.verbose)
    if args.verbose:
        print(npdata)


if __name__ == '__main__':
    main()
