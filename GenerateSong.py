baseA = 330
lowestC = baseA*((2**-4)*(2**(-9/12)))
octaves = 8

intSize = 16

noteNames = {
    0 : "C",
    1 : "C#",
    2 : "D",
    3 : "D#",
    4 : "E",
    5 : "F",
    6 : "F#",
    7 : "G",
    8 : "G#",
    9 : "A",
    10 : "A#",
    11 : "B"
}

noteList = []
freqList = []
currFreq = lowestC

prescalar = 5
ucFreq = 64000000
toneLength = 0.15
toneSteps = 8
restVal = 2500


def freqToARR(freq):
    if freq == 0:
        return 2**intSize-1
    p = 1/freq
    p /= toneSteps
    ARR = int(ucFreq*p/(prescalar+1)-1)

    if ARR >= 2**(intSize):
        print("ARR too large for:",freq)
    return ARR

def rythm2Count(freq,length):
    if freq == 0:
        p=1/nd["A4"]
    else:
        p=1/freq
    p/=toneSteps
    count = int(length*toneLength/p)
    if count >= 2**(intSize):
        print("ARR too large for:",freq)
    return count

for i in range(octaves):
    for j in range(12):
        noteList.append(noteNames[j]+str(i))
        freqList.append(currFreq)
        currFreq *= 2**(1/12)

noteList.append("R")
freqList.append(0)
#Note dictionary
nd = dict(zip(noteList,freqList))

#Hall of the mountain king
"""
melody = [
    ["B4",1], #Takt 1
    ["C#4",1],
    ["D4",1],
    ["E4",1],
    ["F#4",1],
    ["D4",1],
    ["F#4",2], 
    ["F4",1], #Takt 2
    ["C#4",1],
    ["F4",2],
    ["E4",1],
    ["C4",1],
    ["E4",2],
    ["B4",1], #Takt 3
    ["C#4",1],
    ["D4",1],
    ["E4",1],
    ["F#4",1],
    ["D4",1],
    ["F#4",1],
    ["B5",1], 
    ["A5",1], #Takt 4
    ["F#4",1],
    ["D4",1],
    ["F#4",1],
    ["A5",4],
]"""

#Megalovania
melody = [
    ["F4",1], #Takt 1
    ["F4",1],
    ["D5",2],
    ["A4",2],
    ["R",1],
    ["G#4",1],
    ["R",1],
    ["G4",1],
    ["F4",2],
    ["D4",1],
    ["F4",1],
    ["G4",1],
    ["C4",1], #Takt 2
    ["C4",1],
    ["D5",2],
    ["A4",2],
    ["R",1],
    ["G#4",1],
    ["R",1],
    ["G4",1],
    ["F4",2],
    ["D4",1],
    ["F4",1],
    ["G4",1],
    ["B3",1], #Takt 3
    ["B3",1],
    ["D5",2],
    ["A4",2],
    ["R",1],
    ["G#4",1],
    ["R",1],
    ["G4",1],
    ["F4",2],
    ["D4",1],
    ["F4",1],
    ["G4",1],
    ["A#3",1], #Takt 4
    ["A#3",1],
    ["D5",2],
    ["A4",2],
    ["R",1],
    ["G#4",1],
    ["R",1],
    ["G4",1],
    ["F4",2],
    ["D4",1],
    ["F4",1],
    ["G4",1],
    ["F5",2], #Takt 5 (Har sprunget noget over og er gået direkte til hoved melodi)
    ["F5",1],
    ["F5",1],
    ["R",1],
    ["F5",1],
    ["R",1],
    ["F5",2],
    ["D5",1],
    ["R",1],
    ["D5",3],
    ["F5",2], #Takt 6
    ["F5",1],
    ["F5",1],
    ["R",1],
    ["G5",1],
    ["R",1],
    ["G#5",2],
    ["G5",0.5],
    ["G#5",0.5],
    ["G5",1],
    ["D5",1],
    ["F5",1],
    ["G5",1],
    ["R",2],
    ["F5",2], #Takt 7
    ["F5",1],
    ["F5",1],
    ["R",1],
    ["G5",1],
    ["R",1],
    ["G#5",1],
    ["R",1],
    ["A5",1],
    ["R",1],
    ["C6",1],
    ["R",1],
    ["A5",3],
    ["D6",2], #Takt 8
    ["D6",2], 
    ["D6",1], 
    ["A5",1], 
    ["D6",1], 
    ["C5",9],
]

outStringTones = "uint16_t notesT[{}] = ".format(len(melody)) + "{"
outStringRythm = "uint16_t notesR[{}] = ".format(len(melody)) + "{"
for i in melody:
    outStringTones += str(hex(freqToARR(nd[i[0]]))) + ","
    outStringRythm += str(hex(rythm2Count(nd[i[0]],i[1]))) + ","
outStringTones += "};"
outStringRythm += "};"
print("#define nNotes {}".format(len(melody)))
print("#define resetSize {}".format(freqToARR(nd["A4"])))
print(outStringTones)
print(outStringRythm)

