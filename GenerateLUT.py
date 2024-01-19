import math as m
import os
from rig.type_casts import float_to_fp, fp_to_float

funcName = "SQRT"
lutType = "uint32_t"

full = 32
deci = 8

steps = 512

def func(x):
    return m.sqrt(x)


def to_fixed(f,e):
    a = f* (2**e)
    b = int(round(a))
    if a < 0:
        # next three lines turns b into it's 2's complement.
        b = abs(b)
        b = ~b
        b = b + 1
    return b
numbersSimulated = []
for i in range(6):
    for j in range(1,16):
        numbersSimulated.append(j<<(i*4))
print(numbersSimulated)
#numbersSimulated = list(range(0,steps))

s = float_to_fp(signed=True, n_bits=full, n_frac=deci)


outString = "{} {}[{}] = ".format(funcName,lutType,len(numbersSimulated)) +"{"
for i in numbersSimulated:
    outString += str(hex(s(func(i))&0xFFFFFFFF)) +","
outString += "};"
fileName = os.path.dirname(os.path.abspath(__file__))+"\\LUToutput\\"+"{}.txt".format(funcName)

print(fileName)
f = open(fileName,"w+")
f.write(outString)
f.close()
print("done")