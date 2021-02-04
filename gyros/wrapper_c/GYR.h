/* FXAS21002C gyroscope

 */
#ifndef GYR_H_
#define GYR_H_
#define COUNT_TEMP_BIAS               1

// register addresses FXAS21002C_H_
#define FXAS21002C_H_OUT_X_MSB        0x01    
#define FXAS21002C_H_CTRL_REG0        0x0D  
#define FXAS21002C_H_TEMP             0x12
#define FXAS21002C_H_CTRL_REG1        0x13
#define FXAS21002C_H_INT_SRC_FLAG     0x0B

// gyro parameters
#define ODR_VALUE                     12.5
#define ODR_NUM                       0b110 
#define FSR_VALUE                     250
#define FSR_NUM                       0b10
#define SENSITIVITY                   7.8125e-3
#define TEMP_BIAS_COE_X               0.02
#define TEMP_BIAS_COE_Y               0.02
#define TEMP_BIAS_COE_Z               0.01
#define TEMP_SENS_COE_X               0.0008
#define TEMP_SENS_COE_Y               0.0008
#define TEMP_SENS_COE_Z               0.0001

//// Sensor data
//float gyroX = 0;
//float gyroY = 0;
//float gyroZ = 0;
//int8_t tempData = 0;
//int8_t tempData0 = 0;
//float gBiasX = 0;
//float gBiasY = 0;
//float gBiasZ = 0;
//uint8_t address = 0x20;

//extern tempData;
//extern tempData0;
//extern gyroX, gyroY, gyroZ;
//extern gBiasX, gBiasY, gBiasZ;
//extern address;
extern int8_t tempData;
extern int8_t tempData0;
extern float gyroX, gyroY, gyroZ;
extern float gBiasX, gBiasY, gBiasZ;
extern uint8_t address;

void readReg(uint8_t reg, uint8_t *value);
void readRegs(uint8_t reg, uint8_t count, uint8_t dest[]);
void writeReg(uint8_t reg, uint8_t value);
void writeField(uint8_t reg, int bit, int numBit, uint8_t value);
void readTempData(int8_t *tempData);
void setConfigures(void);
void standby(void);
void active(void);
void ready(void);
void readGyroData(float *gyroX, float *gyroY, float *gyroZ, int8_t *tempData);
void calibrate(float *gBiasX, float *gBiasY, float *gBiasZ);
void reset(void);
#endif
