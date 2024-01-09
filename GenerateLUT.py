import math as m
from rig.type_casts import float_to_fp, fp_to_float

funcName = "SQRT"
lutType = "uint32_t"

full = 32
deci = 14

def func(x):
    return m.sin(x*2*m.pi/512)

def to_fixed(f,e):
    a = f* (2**e)
    b = int(round(a))
    if a < 0:
        # next three lines turns b into it's 2's complement.
        b = abs(b)
        b = ~b
        b = b + 1
    return b

numbersSimulated = list(range(0,512))

s = float_to_fp(signed=True, n_bits=full, n_frac=deci)


outString = "{} {}[{}] = ".format(funcName,lutType,len(numbersSimulated)) +"{"
for i in numbersSimulated:
    outString += str(hex(s(func(i))&0xFFFFFFFF)) +","
outString += "};"
print(outString)
