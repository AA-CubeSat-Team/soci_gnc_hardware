This unit_tests.ino file contains unit tests for the functionality of functions inside the gyroscope wrapper.

There are test functions for each one of the functions inside the wrapper.  
To test a specific test, uncomment that test in setup().

Some functions might have dependencies on other functions. 
Testing the function that has no dependency first, 
then testing functions that use this function later, still makes these unit tests valid.

test_initGyro()
This function is used to initialize the gyro struct handle. 
A field inside struct gyro is designed to see if every field is initialized. 
If this value is true(1), then the gyro handle is initialized. Otherwise, the gyro handle is not.

test_readRegs()
This is a helper function to read the values of registers from the gyroscope. 
As registers 0x07 to 0x22 all have default values when the gyroscope starts, 
reading those values and having no difference between the default value is enough to show this function's validity. 

test_writeReg()
This is also a helper function to write the desired value to a register of the gyroscope. 
To test this function's validity, the test code writes a value to a register (GYRO_CTRL_REG1, in this case) first, 
then reads that register later. Comparing these two values shows the validity of the fuction.

test_startGyro()
This is the function to set the gyroscope to the desired configuration and active the gyroscope to read data. 
As the configurations and the "active" filed for the gyroscope are all set by the register, 
checking the located registers' values can show the validity of the function.

test_resetGyro()
This function is to reset the gyroscope. 
Resetting the gyroscope will resulting all settings of the registers back to their default value. 
The test code starts the gyroscope with a configuration first, then resets the gyroscope. Now the code read the reset registers' values, 
and if they are at their default value, then the reset is successful.

test_readTempData and test_readGyroData
These two functions are used to read the temperature data of the gyroscope and the angular rate data of the gyroscope. 
As there is no default value for these data and their values are also dependent on the environment, eyeballing the validity of the output of these functions can show some degree of functionality of those functions. 