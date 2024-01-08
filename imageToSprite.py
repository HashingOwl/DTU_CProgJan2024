import numpy as np
from PIL import Image

imgName = "C:\\Users\\alber\\OneDrive\\Dokumenter\\GitHub\\DTU_CProgJan2024\\test.png"
spriteName = "spriteTest"
image_file = Image.open(imgName)
image_file = image_file.convert('1')

imageMatrix = np.asarray(image_file)
print(imageMatrix.shape)
sprite = []
countLength = 16-1
count = countLength
numb = 0
icount = 0
for i in range(imageMatrix.shape[0]//2):
    for j in range(imageMatrix.shape[1]):
        if count == -1:
            sprite.append(numb)
            numb = 0
            count = countLength
        numb += (imageMatrix[i*2][j]<<count)
        count -= 1
        if imageMatrix.shape[0] > i*2+1:
            numb += (imageMatrix[i*2+1][j]<<count)
        count -= 1
sprite.append(numb)
print("uint8_t {}x = {};".format(spriteName,imageMatrix.shape[1]))
print("uint8_t {}y = {};".format(spriteName,imageMatrix.shape[0]))
outString = "uint16_t {}[{}] = ".format(spriteName,len(sprite)) + "{"
for i in sprite:
    outString += str(hex(i & 0xffffffff)) + ","
outString += "};"

print(outString)
