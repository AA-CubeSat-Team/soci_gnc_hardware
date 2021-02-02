#last updated 1-18-21
#Sun Sensor General Communication code written by William Lacrampe
#This code is meant to be used to try and get the sun sensor to respond to each command. It does not keep track
#of the data the sun sensor returns; other scripts must be used for that kind of testing. This is just to make sure
#we can use every command with the sun sensor and get something back. The code will repeatedly ask the user which command
#the user wants to try.
import serial
from time import sleep

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
        floatNum = 2
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
    #the third value for an angle reading is actually an error code, need to check
    #if it's supposed to read it, and if so, read it
    if floatNum == 2:
        readVals[2] = int.from_bytes(ser.read(), byteorder='big')
        #also need to add it to checkSum
        checkSum = checkSum + readVals[2]
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

#defining some bytes that will be repeatedly used    
addressByte = b'\x60'
lengthByte = b'\x01'

#writes angle command bytes to sun sensor, checks for proper response, then calls readData and prints out the data
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
    print(ser.in_waiting)
    commByte = ser.read()
    #check that I receive the right command code
    if commByte == b'\x04':
        #pass along the command code
        returnedData = readData(4)
        print('data received')
        print(returnedData)
    else:
        print('wrong command code received')
        #reset the input buffer because the rest of the sun sensor's data
        #should be ignored and removed
        ser.reset_input_buffer()     

#similar to getAngle, but for voltage
def getVoltage():
    ser.write(addressByte)
    ser.write(b'\x01')
    ser.write(lengthByte)
    ser.write(b'\x02')
    sleep(0.25)
    ser.read()
    if ser.read() == b'\x01':
        returnedData = readData(1)
        print('data received')
        print(returnedData)
    else:
        print('wrong command code received')
        ser.reset_input_buffer()

#similar to getAngle, but for filtered voltage
def getFilteredVoltage():
    ser.write(addressByte)
    ser.write(b'\x03')
    ser.write(lengthByte)
    ser.write(b'\x04')
    print('command written')
    sleep(0.25)
    ser.read()
    if ser.read()== b'\x03':
        returnedData = readData(3)
        print('data received')
        print(returnedData)
    else:
        print('wrong command code received')
        ser.reset_input_buffer()

sleep(1)
running = True
while running:
    #prompt user for input
    userIn = input("Give command: voltage (V), filtered voltage (F), or angles (A) or quit (Q): ")
    if userIn == "V":
        print('getting voltage')
        getVoltage();
    elif userIn == "F":
        print('getting filtered voltage')
        getFilteredVoltage();
    elif userIn == "A":
        print('getting angles')
        getAngles();
    elif userIn == "Q":
        running = False
    else:
        print("Command not recognized, please try again.")
