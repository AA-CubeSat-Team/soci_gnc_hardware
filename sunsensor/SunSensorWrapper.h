/* initializes sun sensor UART */
/* note this has nothing to do with actually turning the sun sensor on */
/* I think that the higher level flight software might need this so I think */
/* it should be in the header */
void sunSenUARTInit();

/* this is the main function that higher level flight code should call */
/* it will always get new angles and occasionally get new cell voltage */
/* it takes 3 pointers: the first is that of the the first element of the angles, the second is the same but of filtered volts, */
/* and the third is the same but of unfiltered volts */
/* if you try to pass the pointer for an entire array it gets upset for some reason */
void getData(float* angles, float* filtVolts, float* unFiltVolts);
