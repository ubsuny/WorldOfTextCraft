import json
with open('battlelog.txt') as json_file:
    data = json.load(json_file)

charnames = ["Arthas", "Fordring", "Thrall", "Mograine"]


turns = data["Turns"]
turnheaders = [ s for s in turns[0][charnames[0]] ]
print(turnheaders)



for turn in turns:
    turnval = turn["Turn"]
    print ("------------ turn : ", turnval, " ---------------")
    for charname in charnames:
        s = "==== %10s :" % (charname)
        vals = turn[charname]
        for iact,val in vals.items():
            s += "%3d," % (sum(val) )
        print(s)
