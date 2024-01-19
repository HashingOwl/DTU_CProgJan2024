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
toneLength = 0.25
toneSteps = 32
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

#Bad Apple
melody = [
    ["A2",1], #Takt 1
    ["B2",1], 
    ["C3",1],
    ["D3",1], 
    ["E3",2],
    ["A3",1],  
    ["G3",1],
    ["E3",2], #Takt 2
    ["A2",2],
    ["A3",1],  
    ["G3",1],
    ["C3",1],
    ["B2",1],
    ["A2",1], #Takt 3
    ["B2",1], 
    ["C3",1],
    ["D3",1], 
    ["E3",2],
    ["D3",1], 
    ["C3",1], 
    ["B2",1], #Takt 4
    ["A2",1], 
    ["B2",1], 
    ["C3",1], 
    ["B2",1],
    ["A2",1],  
    ["G#2",1], 
    ["B2",1], 
    ["A2",1], #Takt 5
    ["B2",1], 
    ["C3",1],
    ["D3",1], 
    ["E3",2],
    ["A3",1],  
    ["G3",1],
    ["E3",2], #Takt 6
    ["A2",2],
    ["A3",1],  
    ["G3",1],
    ["C3",1],
    ["B2",1],
    ["A2",1], #Takt 7
    ["B2",1], 
    ["C3",1],
    ["D3",1], 
    ["E3",2],
    ["D3",1], 
    ["C3",1], 
    ["B2",1], #Takt 8
    ["R",1], 
    ["C3",1], 
    ["R",1], 
    ["D3",2],
    ["E3",4],  
    ["A2",1], #Takt 9
    ["B2",1], 
    ["C3",1],
    ["D3",1], 
    ["E3",2],
    ["A3",1],  
    ["G3",1],
    ["E3",2], #Takt 10
    ["A2",2],
    ["A3",1],  
    ["G3",1],
    ["C3",1],
    ["B2",1],
    ["A2",1], #Takt 11
    ["B2",1], 
    ["C3",1],
    ["D3",1], 
    ["E3",2],
    ["D3",1], 
    ["C3",1], 
    ["B2",1], #Takt 12
    ["A2",1], 
    ["B2",1], 
    ["C3",1], 
    ["B2",1],
    ["A2",1],  
    ["G#2",1], 
    ["B2",1], 
    ["A2",1], #Takt 13
    ["B2",1], 
    ["C3",1],
    ["D3",1], 
    ["E3",2],
    ["A3",1],  
    ["G3",1],
    ["E3",2], #Takt 14
    ["A2",2],
    ["A3",1],  
    ["G3",1],
    ["C3",1],
    ["B2",1],
    ["A2",1], #Takt 15
    ["B2",1], 
    ["C3",1],
    ["D3",1], 
    ["E3",2],
    ["D3",1], 
    ["C3",1], 
    ["B2",1], #Takt 16
    ["R",1], 
    ["C3",1], 
    ["R",1], 
    ["D3",2], 
    ["E3",4], 
    ["D3",1], #Takt 17
    ["E3",1],
    ["G3",1],
    ["A3",0.5],
    ["G3",0.5],
    ["E3",1],
    ["D3",1],
    ["E3",2],
    ["D3",1], #Takt 18
    ["E3",1],
    ["G3",1],
    ["A3",0.5],
    ["G3",0.5],
    ["E3",1],
    ["D3",1],
    ["E3",2],
    ["G3",1], #Takt 19
    ["A3",1], 
    ["G3",1],
    ["C3",1],  
    ["B2",1], 
    ["G2",1],
    ["A2",2],  
    ["G2",1], #Takt 20
    ["A2",1], 
    ["B2",1], 
    ["C3",1], 
    ["D3",1], 
    ["E3",1], 
    ["A2",2], 
    ["D3",1], #Takt 21
    ["E3",1],
    ["G3",1],
    ["A3",0.5],
    ["G3",0.5],
    ["E3",1],
    ["D3",1],
    ["E3",2],
    ["D3",1], #Takt 22
    ["E3",1],
    ["G3",1],
    ["A3",0.5],
    ["G3",0.5],
    ["E3",1],
    ["D3",1],
    ["E3",2],
    ["G3",1], #Takt 23
    ["A3",1], 
    ["G3",1],
    ["C3",1],  
    ["B2",1], 
    ["G2",1],
    ["A2",2],  
    ["G2",1], #Takt 24
    ["A2",1], 
    ["B2",1], 
    ["C3",1], 
    ["D3",1], 
    ["E3",1], 
    ["A2",2], 
    ["D4",1], #Takt 25
    ["E4",1],
    ["G4",1],
    ["A4",0.5],
    ["G4",0.5],
    ["E4",1],
    ["D4",1],
    ["E4",2],
    ["D4",1], #Takt 26
    ["E4",1],
    ["G4",1],
    ["A4",0.5],
    ["G4",0.5],
    ["E4",1],
    ["D4",1],
    ["E4",2],
    ["D4",1], #Takt 27
    ["E4",1],
    ["D4",1],
    ["C4",1],
    ["B3",0.5],
    ["C4",0.1],
    ["G3",1],
    ["A3",2],
    ["G3",1], #Takt 28
    ["A3",1],
    ["B3",1],
    ["C4",1],
    ["D4",0.5],
    ["C4",0.5],
    ["E4",1],
    ["A3",1],
    ["D3",1], #Takt 29
    ["E3",1],
    ["G3",1],
    ["A3",0.5],
    ["G3",0.5],
    ["E3",1],
    ["D3",1],
    ["E3",2],
    ["D3",1], #Takt 30
    ["E3",1],
    ["G3",1],
    ["A3",0.5],
    ["G3",0.5],
    ["E3",1],
    ["D3",1],
    ["E3",2],
    ["A3",1], #Takt 31
    ["B3",1],
    ["C4",1],
    ["B3",1],
    ["A3",1],
    ["G3",1],
    ["E3",2],
    ["D3",1], #Takt 32
    ["E3",1],
    ["D3",1],
    ["C3",1],
    ["B2",1],
    ["G2",1],
    ["A2",1],
    ["A2",0.25], #Takt 33
    ["R",0.25],
    ["A2",1.25],
    ["R",0.25],
    ["A2",0.75],
    ["R",0.25],
    ["A2",1.25],
    ["R",0.25],
    ["A2",1.25],
    ["R",0.25],
    ["A2",0.75],
    ["R",0.25],
    ["A2",1.25], #Takt 34
    ["R",0.25],
    ["A2",1.25],
    ["R",0.25],
    ["A2",0.75],
    ["R",0.25],
    ["A2",1.25],
    ["R",0.25],
    ["A2",1.25],
    ["R",0.25],
    ["A2",1],
    ["R",1],
    ["R",16] #Takt 35-36
]

#Flowering Night
melody2 = [
    ["A2",1], #Takt 1
    ["F3",1],
    ["E3",0.5],
    ["F3",0.5],
    ["E3",0.5],
    ["F3",0.5],
    ["G3",1], #Takt 2
    ["F3",1],
    ["E3",0.5],
    ["F3",0.5],
    ["E3",0.5],
    ["C3",0.5],
    ["D3",1], #Takt 3
    ["A3",1],
    ["G3",0.5],
    ["A3",0.5],
    ["G3",0.5],
    ["A3",0.5],
    ["C4",1], #Takt 4
    ["A3",1],
    ["G3",1],
    ["A3",1],
    ["A2",1], #Takt 5
    ["F3",1],
    ["E3",0.5],
    ["F3",0.5],
    ["E3",0.5],
    ["F3",0.5],
    ["G3",1], #Takt 6
    ["F3",1],
    ["E3",0.5],
    ["F3",0.5],
    ["E3",0.5],
    ["C3",0.5],
    ["D3",1], #Takt 7
    ["A3",1],
    ["G3",0.5],
    ["A3",0.5],
    ["G3",0.5],
    ["A3",0.5],
    ["C#4",1], #Takt 8
    ["A3",1],
    ["G3",1],
    ["A3",1],
    ["A2",1], #Takt 9
    ["F3",1],
    ["E3",0.5],
    ["F3",0.5],
    ["E3",0.5],
    ["F3",0.5],
    ["G3",1], #Takt 10
    ["F3",1],
    ["E3",0.5],
    ["F3",0.5],
    ["E3",0.5],
    ["C3",0.5],
    ["D3",1], #Takt 11
    ["A3",1],
    ["G3",0.5],
    ["A3",0.5],
    ["G3",0.5],
    ["A3",0.5],
    ["C4",1], #Takt 12
    ["A3",1],
    ["G3",1],
    ["A3",1],
    ["A2",1], #Takt 13
    ["F3",1],
    ["E3",0.5],
    ["F3",0.5],
    ["E3",0.5],
    ["F3",0.5],
    ["G3",1], #Takt 14
    ["F3",1],
    ["E3",0.5],
    ["F3",0.5],
    ["E3",0.5],
    ["C3",0.5],
    ["D3",1], #Takt 15
    ["A3",1],
    ["G3",0.5],
    ["A3",0.5],
    ["G3",0.5],
    ["A3",0.5],
    ["C4",1], #Takt 16
    ["C4",1],
    ["D4",2],
    ["E4",1], #Takt 17 (Takt 19 i noderne)
    ["G3",1],
    ["G4",1],
    ["G3",1],
    ["F#4",1], #Takt 18
    ["B3",1],
    ["E4",1],
    ["D#4",2], #Takt 19
    ["B3",1], 
    ["B4",1],
    ["B3",1],
    ["F#4",1], #Takt 20
    ["B3",1],
    ["D#4",1],
    ["B3",1],
    ["E4",1], #Takt 21
    ["G3",1],
    ["G4",1],
    ["G3",1],
    ["F#4",1], #Takt 22
    ["B3",1],
    ["E4",1],
    ["D#4",2], #Takt 23
    ["B3",1], 
    ["B4",1],
    ["B3",1],
    ["D#4",3], #Takt 24
    ["E4",2], #Takt 25 
    ["G3",1],
    ["G4",1],
    ["G3",1],
    ["F#4",1], #Takt 26
    ["B3",1],
    ["E4",1],
    ["D#4",2], #Takt 27
    ["B3",1], 
    ["B4",1],
    ["B3",1],
    ["F#4",1], #Takt 28
    ["B3",1],
    ["D#4",1],
    ["B3",1],
    ["E4",1], #Takt 29
    ["F#4",1],
    ["G4",1],
    ["A4",2], #Takt 30
    ["B4",1],
    ["D5",1],
    ["E5",5],
    ["R",16]
]

for i in melody2:
    melody.append(i)
#melody = melody2

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

