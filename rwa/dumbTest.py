

rplArr = [0x7e, 0x7e, 0x7e, 0x7e, 0x03, 0x01, 0xe5, 0xe2, 0x7e, 0x7e, 0x7e, 0x7e]

bytOld = 0x7e
for idx, byt in enumerate(rplArr):
    bytNew = byt
    if (bytOld == 0x7e) & (bytNew != 0x7e):
        idxStart = idx
        print('start: ',idxStart)
    if (bytOld != 0x7e) & (bytNew == 0x7e):
        idxEnd = idx - 1
        print('end: ',idxEnd)
    bytOld = bytNew

rplArr1 = rplArr[idxStart:(idxEnd+1)] 
print(rplArr1)



