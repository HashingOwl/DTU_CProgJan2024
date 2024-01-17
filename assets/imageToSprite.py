import numpy as np
import os
from PIL import Image

#imgName = "C:\\Users\\alber\\OneDrive\\Dokumenter\\GitHub\\DTU_CProgJan2024\\alien3_2.png"
 
spriteName = "boss"
imgName = os.path.dirname(os.path.abspath(__file__))+"\\"+spriteName+".png"
image_file = Image.open(imgName)
image_file = image_file.convert('1')

imageMatrix = np.asarray(image_file)
print(imageMatrix.shape)
sprite = []
countLength = 8-1
count = 0
numb = 0
icount = 0
"""
for j in range(imageMatrix.shape[1]):
    for i in range(imageMatrix.shape[0]//2):
        if count == -1:
            sprite.append(numb)
            numb = 0
            count = countLength
        numb += (imageMatrix[i*2][j]<<count)
        count -= 1
        if imageMatrix.shape[0] > i*2+1:
            numb += (imageMatrix[i*2+1][j]<<count)
        count -= 1
sprite.append(numb)"""

for y in range(imageMatrix.shape[0]//2):
    for x in range(imageMatrix.shape[1]):
        numb += (imageMatrix[y*2][x]<<count)
        count += 1
        if y*2+1<imageMatrix.shape[0]:
            numb += (imageMatrix[y*2+1][x]<<count)
        count += 1
        if count >= countLength:
            sprite.append(numb)
            numb = 0
            count = 0
print("uint8_t {}x = {};".format(spriteName,imageMatrix.shape[1]))
print("uint8_t {}y = {};".format(spriteName,imageMatrix.shape[0]))
outString = "const uint8_t {}[{}] = ".format(spriteName,len(sprite)) + "{"
count = 0
for i in sprite:
    outString += "0x"+f"{(i & 0xffffffff):02x}" + "," +"\t"
    count += 1
    if count == 16:
        count = 0
        outString += "\n"
outString += "};"


fileName = os.path.dirname(os.path.abspath(__file__))+"\\output\\"+"{}.txt".format(spriteName)

print(fileName)
f = open(fileName,"w+")
f.write(outString)
f.close()
print("done")
#print(outString)
