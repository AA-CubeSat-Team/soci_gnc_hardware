/* FXAS21002C
 *
 *Basada en:
 * FXOS8700CQ
 *
 *
 * The FXAS21002C is an gyroscope I2C sensor which is 3.3V tolerant.
 */


#ifndef FXAS21002C_H_
#define FXAS21002C_H_

#include <Arduino.h> // for byte data type

// register addresses FXAS21002C_H_
#define FXAS21002C_H_STATUS           0x00
#define FXAS21002C_H_DR_STATUS        0x07
#define FXAS21002C_H_F_STATUS         0x08
#define FXAS21002C_H_OUT_X_MSB        0x01    
#define FXAS21002C_H_OUT_X_LSB        0x02
#define FXAS21002C_H_OUT_Y_MSB        0x03
#define FXAS21002C_H_OUT_Y_LSB        0x04
#define FXAS21002C_H_OUT_Z_MSB        0x05
#define FXAS21002C_H_OUT_Z_LSB        0x06
#define FXAS21002C_H_F_SETUP          0x09
#define FXAS21002C_H_F_EVENT          0x0A
#define FXAS21002C_H_INT_SRC_FLAG     0x0B
#define FXAS21002C_H_WHO_AM_I         0x0C
#define FXAS21002C_H_CTRL_REG0        0x0D  
#define FXAS21002C_H_RT_CFG       	  0x0E
#define FXAS21002C_H_RT_SRC       	  0x0F 
#define FXAS21002C_H_RT_THS       	  0x10
#define FXAS21002C_H_RT_COUNT         0x11
#define FXAS21002C_H_TEMP             0x12
#define FXAS21002C_H_CTRL_REG1        0x13
#define FXAS21002C_H_CTRL_REG2        0x14
#define FXAS21002C_H_CTRL_REG3        0x15

enum GyroODR {
	GODR_800HZ = 0,
	GODR_400HZ,
	GODR_200HZ,
	GODR_100HZ,
	GODR_50HZ,
	GODR_25HZ,
	GODR_12_5HZ,

};
// Set initial input parameters
enum GyroFSR {
  GFSR_2000DPS = 0,
  GFSR_1000DPS,
  GFSR_500DPS,
  GFSR_250DPS,
};

enum GyroBW {
  GBW_L1 = 0,
  GBW_L2,
  GBW_L3,
};

class FXAS21002C
{
	public:
	typedef struct 
	{
	    float	x;
	    float	y;
	    float	z;
	} SRAWDATA;

	// Sensor data
	SRAWDATA gyroData; // RAW acceleration sensor data
	int8_t tempData; // RAW temperature data

	// Sensor configuration
	uint8_t gyroFSR;
	uint8_t gyroODR;
	uint8_t gyroBW;
	float gBias[3] = {0,0,0};

	FXAS21002C(GyroFSR fsr, GyroODR odr, GyroBW bw);

	// Register functions
	void writeReg(byte reg, byte value);
	byte readReg(byte reg);
	void writeField(byte reg, byte bit, byte value);
	void readRegs(byte startReg, uint8_t count, byte dest[]);

	// FXAS21002C functions
	// Initialization & Termination
	void setConfigures(void);
	void standby(void);
	void active(void);
	void ready(void);

	// Query sensor data
	void readGyroData(void);
	void readTempData(void);

	
	// output data rate
	double getODR(void);

	//Calibrate
	void calibrate();

	//Reset
	void reset(void);

	private:
	// Sensor address
	byte address = 0x20;

	// Sensitivity
	float sensitivity(void);
};

#endif
