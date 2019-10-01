import json
import numpy as np


with open('battlelog.txt') as json_file:
    data = json.load(json_file)


turns = data["Turns"]
charnames = list(turns[0].keys())
charnames.remove("Turn")
turnheaders = [ s for s in turns[0][charnames[0]] ]
print(turnheaders)

nturns = len( data["Turns"] )

npdata = np.zeros( (len(charnames),nturns,len(turnheaders)) )

for iturn,turn in enumerate( turns ):
    turnval = turn["Turn"]
    print ("------------ turn : ", turnval, " ---------------")
    for icharname, charname in enumerate(charnames):
        s = "==== %10s :" % (charname)
        vals = turn[charname]
        for iact,val in vals.items():
            s += "%3d," % (sum(val) )        
            npdata[icharname,turnval,turnheaders.index(iact)] = sum(val)
        print(s)

print(npdata)
