

inpString = input("enter hex bytes with no spaces: \n")

inpCharList = list(inpString)

inpLength = len(inpCharList)
txLength = int(inpLength / 2)

txByteArray = [0] * txLength

for c in range(0, txLength):
    txByteArray[c] = inpCharList[2*c] + inpCharList[(2*c)+1]
    txByteArray[c] = int(txByteArray[c],16)

print(hex(txByteArray))


#txHex = hex(txDec)


