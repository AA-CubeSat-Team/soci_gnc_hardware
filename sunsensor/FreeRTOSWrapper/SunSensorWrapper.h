/* various constants used in the sun sensor wrapper */
/* command bytes to be issued to sun sensor */
const uint8_t unFiltVoltsComm[4] = {0x60, 0x01, 0x01, 0x02};
const uint8_t filtVoltsComm[4] = {0x60, 0x03, 0x01, 0x04};
const uint8_t anglesComm[4] = {0x60, 0x04, 0x01, 0x05};

/* delay times in ticks needed */
const TickType_t xDelay3ms = pdMS_TO_TICKS(3);
const TickType_t xDelay7ms = pdMS_TO_TICKS(7);
/* this is the minimum possible time between commands - the typical time is 50 ms, I might want to change this */
const TickType_t xDelay20ms = pdMS_TO_TICKS(20);

/* lengths of voltage responses and angle response */
const int voltRespLength = 20;
const int angleRespLength = 17;

/* initializes sun sensor UART */
/* note this has nothing to do with actually turning the sun sensor on */
void sunSenUARTInit();

/* gets the angles from the sun sensor */
/* only give it a pointer to the first angle element, not a pointer to the entire array */
void getSunAngles(double* angles);

/* checks the sun sensor health */
/* needs a pointer to the sun sensor's health */
void checkSunSensorHealth(double* volts);