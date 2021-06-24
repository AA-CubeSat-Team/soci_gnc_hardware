#last updated 1-18-21
#Response Timer code written by William Lacrampe
#This code will repeatedly prompt the sun sensor using a specific command given by the user and time how long
#it takes for the sun sensor to respond. It will also prompt the user to name a file to save the response times to.
import serial
import time
import csv

ser = serial.Serial(port="/dev/ttyS0", baudrate=115200, bytesize=serial.EIGHTBITS,
                    parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE)

#times how long it takes for the sun sensor to respond to a specific command, given by comm, and measures it numReads times.
#then writes the time to a file for later use
def timer(comm, numReads):
    #name the file
    filename = input("Please give a name for the file: ") +".csv"
    #start up the writer
    with open(filename, 'w') as file:
        writer = csv.writer(file)
        writer.writerow(["Time to response"])
        #check what command it is, specifies what the bytes to write will be
        #bytes will default to voltage or "V"
        commByte = b'\x01'
        checkSum = b'\x02'
        #these next two bytes are the same for all commands
        addressByte = b'\x60'
        lengthByte = b'\x01'
        #check if the user wants filtered voltage
        if comm == "F":
            commByte = b'\x03'
            checkSum = b'\x04'
        #otherwise, check if the user wants angles
        elif comm == "A":
            commByte = b'\x04'
            checkSum = b'\x05'
        #don't need to check if they want voltage since the bytes default to that and there's no other option
        #repeatedly contact the sun sensor and time how long it takes for a response, then write that time
        for x in range(numReads):
            #issue the command
            ser.write(addressByte)
            ser.write(commByte)
            ser.write(lengthByte)
            ser.write(checkSum)
            #get starting time
            startTime = time.time()
            #wait until there is a response
            timeout = False
            while ser.in_waiting == 0:
                #do nothing while waiting, make sure that the sun sensor isn't taking too long to respond though
                #these times are on the order of 1/100th a second at most, an entire second would be absurd
                if time.time()- startTime > 1:
                    #inform user and end the waiting early
                    print("Timeout error. Ending reading.")
                    timeout = True
                    break
            #see how long it's taken to get to this point
            responseTime = time.time()-startTime
            #if it didn't time out, write the time it took to respond
            if timeout == False:
                writer.writerow([responseTime])
            #then reset the input buffer since we're not actually using it for anything
            ser.reset_input_buffer()
            #then wait a little bit
            time.sleep(0.05)
        print("Reading done")

time.sleep(1)
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
        timer(comm, numReads)
        gettingComm = False
