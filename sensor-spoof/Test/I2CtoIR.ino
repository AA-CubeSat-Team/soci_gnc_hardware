#include <Wire.h>
#include <IRremote.h>

int lowByte;
int highByte;
int value;

IRsend irsend;

void setup() {
	Wire.begin(8);                // join i2c bus with address x (type int)
	Wire.onReceive(receiveEvent); // register event
	Serial.begin(9600);
	Serial.println("Boot OK");
}

void loop() {
  // Nothing here
}

void receiveEvent(int howMany) {
	if (Wire.available() == 2) {
		int lowByte = Wire.read();
		int highByte = Wire.read();
		int value = highByte * 256 + lowByte;
		sendIR(value);
	}
}

void sendIR(int n) {
	Serial.print("Invio segnale, codice: ");
	Serial.println(n);
	for(int i=0; i<3; i++){
		irsend.sendSony(n, 12);
		delay(40);
	}
	Serial.println("Inviato");
}
