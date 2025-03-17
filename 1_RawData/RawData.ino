#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(1, 0); // RX=1, TX=0

void setup() {
  Serial.begin(115200);  // Debugging
  gpsSerial.begin(9600); // GPS baud rate
  Serial.println("Waiting for GPS data...");
}

void loop() {
  while (gpsSerial.available()) {
	char c = gpsSerial.read();
	Serial.print(c);  // Print raw GPS data
  }
}
