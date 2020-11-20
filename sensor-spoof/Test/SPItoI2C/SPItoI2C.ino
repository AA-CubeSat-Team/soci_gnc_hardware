#include "respeaker.h"
#include <Wire.h>

int TVcode[11] = {2704,16,2064,1040,3088,528,2576,1552,3600,272,2320};

void setup() {
	// ReSpeaker
	respeaker.begin();
	respeaker.attach_spi_handler(spi_event);

	// Wire
	Wire.begin(); // join i2c bus (address optional for master)

	Serial.begin(57600);
	Serial.println("Boot OK");
}

void loop() {
	// Nothing here
}

void spi_event(uint8_t addr, uint8_t *data, uint8_t len) {
	// Display the data, debug purpose only
	Serial.print("Indirizzo");
	Serial.println(addr);
	Serial.print("Data");
	Serial.print(data[0]);
	Serial.print("Lunghezza");
	Serial.println(len);

	// If the first 3 byte are "1", then send the last to TV
	if(data[0] == 1 && data[1] == 1 && data[2] == 1){
	    sendI2C(TVcode[data[3]], 8); // Pass TV code to the function
	    Serial.print("Byte sequence OK, sending...");
	}
}

void sendI2C(int data, int device) {
	Wire.beginTransmission(device);  // transmit to device 
	Wire.write(lowByte(data));       // sends an int
	Wire.write(highByte(data)); 
	Wire.endTransmission();          // stop transmitting
	Serial.println("Send");
}