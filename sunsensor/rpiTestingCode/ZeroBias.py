#last updated 4-22-21
#Zero Bias code written by William Lacrampe
#The zero bias code is meant to test if the sun sensor has any inherit offset in any of the data
#it can return. This should be done in a completely dark enviornment. The code will ask the user to
#name a file that it will save the responses to, and ask what command the user would like to test,
#and how many times the user would like to test it. It will then repeatedly contact the sun sensor and
#save its responses to the named file.
import serial
from time import sleep
import csv

ser = serial.Serial(port="/dev/ttyS0", baudrate=115200, bytesize=serial.EIGHTBITS,
                    parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE)

#Takes a command code corresponding to the command as input
#1 = voltage, 3 = filtered voltage, 4 = angles
#It reads data bytes and interprets them and returns the interpreted values
def readData(commandCode):
    #this will hold interpreted values
    readVals = [0, 0, 0, 0]
    #checkSum is to make sure data is properly read
    #it is the sum of commandCode + length + (all the data bytes)
    #Eventually truncated to the least significant byte
    #to compare with the checkSum byte the sensor passes along
    checkSum = commandCode
    #need to choose the right amount of floats to read, initialize at 0
    #it's either 2 for angles (1 float per angle) or 4 for voltage (1 per cell)
    floatNum = 0
    if commandCode == 4:
        floatNum = 3
    else:
        floatNum = 4
    #sun sensor also hands along length, which is the next byte:
    checkSum = checkSum + int.from_bytes(ser.read(), byteorder = 'big')
    for x in range(1, floatNum+1):
        #need to define some variables that will be used later
        sign = -1
        exp = 0
        frac = 1.0
        #need to get the 4 bytes that will give one of the floats
        inputBytes = [ser.read(), ser.read(), ser.read(), ser.read()]
        #I've got to put these into bits and then put them in the right order
        #currently they're reversed
        for i in range(0,4):
            #first translate the byte as an int
            inputBytes[i] = int.from_bytes(inputBytes[i], byteorder='big')
            #then add it to checkSum (used at the end)
            checkSum = checkSum + inputBytes[i]
            #then put them into "bits" that are actually strings
            #this is horribly expensive and won't be done on actual flight code
            #hopefully C has better options for bit operations
            inputBytes[i] = bin(inputBytes[i])
            #when I do this, I add a prefix '0b' to every input byte that I need to remove
            inputBytes[i] = inputBytes[i].replace('0b','')
            #then pad the bits with 0s since they are important
            for k in range(1, 9-len(inputBytes[i])):
                inputBytes[i] = '0' + inputBytes[i]
        #combine all of the strings into one long string
        longForm = inputBytes[3] + inputBytes[2] + inputBytes[1] + inputBytes[0]
        #the first "bit" of longForm is the sign bit, which defaults to -1, so I only need to check
        #the case where it should be 1:
        if longForm[0] == '0':
            sign = 1
        #the next 8 "bits" of longForm give exp
        exp = int(longForm[1:9],2)
        #the last bits give the fraction, which is found by
        #frac = 1 + sum (from i = 9 to i = 31) b_i*2^(-(i-8))
        #keep in mind frac initializes as 1
        for i in range(9,32):
            #the 0 bits don't count so I can just check for 1s
            if longForm[i] == '1':
                frac = frac + 2**(-(i-8))
        #lastly, put it all through the formula:
        readVals[x-1] = sign*2**(exp-127)*frac
    #the fourth value for an angle reading is actually an error code, need to check
    #if it's supposed to read it, and if so, read it
    if floatNum == 3:
        readVals[3] = int.from_bytes(ser.read(), byteorder='big')
        #also need to add it to checkSum
        checkSum = checkSum + readVals[3]
    #Need to cut checkSum down to just the least significant byte
    #A byte can be no greater than 255, and 256 is a factor of 2^(8+i) nonnegative i
    #so check if checkSum is greater than 255 and remove 256
    while checkSum > 255:
        checkSum = checkSum - 256
    #then I read the checkSum byte from the sensor and check if it's different
    #if it is, then return a bunch of -100's
    sensorCheckSum = int.from_bytes(ser.read(), byteorder='big')
    if checkSum != sensorCheckSum:
        print('checkSum is bad')
        #-1000 volts would be both the wrong sign and ridiculously large for voltage
        #and would make no sense for angles and the angle error code
        return [-1000, -1000, -1000, -1000]
    return readVals
    
addressByte = b'\x60'
lengthByte = b'\x01'
def getAngles():
    #write address byte
    ser.write(addressByte)
    #write command code
    ser.write(b'\x04')
    #write length
    ser.write(lengthByte)
    #write checksum - defined as sum of command code and length
    ser.write(b'\x05')
    #wait for a bit to let the sun sensor process
    sleep(0.25)
    #eat the address byte, it's not useful
    ser.read()
    commByte = ser.read()
    #check that I receive the right command code
    if commByte == b'\x04':
        #pass along the command code
        returnedData = readData(4)
        return returnedData
    else:
        #reset the input buffer because the rest of the sun sensor's data
        #should be ignored and removed
        ser.reset_input_buffer()
        return[-2000, -2000, -2000, -2000]
        

def getVoltage():
    ser.write(addressByte)
    ser.write(b'\x01')
    ser.write(lengthByte)
    ser.write(b'\x02')
    sleep(0.25)
    ser.read()
    if ser.read() == b'\x01':
        returnedData = readData(1)
        return returnedData
    else:
        ser.reset_input_buffer()
        return[-2000, -2000, -2000, -2000]

def getFilteredVoltage():
    ser.write(addressByte)
    ser.write(b'\x03')
    ser.write(lengthByte)
    ser.write(b'\x04')
    sleep(0.25)
    ser.read()
    if ser.read()== b'\x03':
        returnedData = readData(3)
        return returnedData
    else:
        ser.reset_input_buffer()
        return[-2000, -2000, -2000, -2000]

#takes a command and a number of times to read in data, then repeatedly calls the command and stores its values in a file
def repeatedRead(comm, numReads):
    #name the file
    filename = input("Please give a name for the file: ") +".csv"
    #start up the writer
    with open(filename, 'w') as file:
        writer = csv.writer(file)
        #check what command it is, read in vals and write them to the file if they are non-error vals
        if comm == "A":
            writer.writerow(["Alpha", "Beta", "Sun Detection", "Error Code"])
            for x in range(numReads):
                print("Reading")
                print(x)
                readVals = getAngles()
                #make sure they're not errors:
                if readVals != [-2000, -2000, -2000] and readVals != [-1000, -1000, -1000]:
                    #write the values
                    writer.writerow(readVals)
        elif comm == "V":
            writer.writerow(["Cell 1", "Cell 2", "Cell 3", "Cell 4"])
            for x in range(numReads):
                readVals = getVoltage()
                if readVals != [-2000, -2000, -2000, -2000] and readVals != [-1000, -1000, -1000, -1000]:
                    writer.writerow(readVals)
        elif comm == "F":
            writer.writerow(["Cell 1", "Cell 2", "Cell 3", "Cell 4"])
            for x in range(numReads):
                readVals = getFilteredVoltage()
                if readVals != [-2000, -2000, -2000, -2000] and readVals != [-1000, -1000, -1000, -1000]:
                    writer.writerow(readVals)
        #inform the user the reading is complete
        print("Reading completed.")

sleep(1)
gettingComm = True
while gettingComm:
    #prompt user for command
    comm = input("Give command: voltage (V), filtered voltage (F), or angles (A) or quit (Q): ")
    #check to make sure they gave a valid input
    if comm != "V" and comm != "F" and comm != "A" and comm != "Q":
        print("Command not recognized, please try again.")
    #check if they want to quit
    elif comm == "Q":
        gettingComm = False
    else:
        #prompt user for number of times to read in values
        numReads = int(input("How many times to read in values? "))
        repeatedRead(comm, numReads)
        gettingComm = False

