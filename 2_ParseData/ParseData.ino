#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(5, 4); 

void setup() {
    Serial.begin(9600);
    gpsSerial.begin(9600); 
    Serial.println("GPS Module Test - Waiting for GPS Fix...");
}

void loop() {
    static char buffer[100];
    static int index = 0;

    while (gpsSerial.available()) {
        char c = gpsSerial.read();

        if (c == '$') { 
            index = 0;  
        }

        if (index < sizeof(buffer) - 1) {
            buffer[index++] = c;
        }

        if (c == '\n') { 
            buffer[index] = '\0';
            if (strstr(buffer, "$GPRMC")) {
                parseGPRMC(buffer);
            }
            index = 0;
        }
    }
}

void parseGPRMC(char *buffer) {
    float lat = 0, lon = 0;
    int hour, minute, second;
    char ns, ew, status;

    int lat_deg, lon_deg;
    float lat_min, lon_min;

    int parsed = sscanf(buffer, "$GPRMC,%2d%2d%2d.%*d,%c,%2d%f,%c,%3d%f,%c",
                        &hour, &minute, &second, &status,
                        &lat_deg, &lat_min, &ns, 
                        &lon_deg, &lon_min, &ew);

    if (parsed >= 9 && status == 'A') { 
        lat = lat_deg + (lat_min / 60.0);
        lon = lon_deg + (lon_min / 60.0);

        if (ns == 'S') lat = -lat;
        if (ew == 'W') lon = -lon;

        Serial.print("Latitude: ");
        Serial.print(lat, 7);
        Serial.print("° ");
        Serial.print(ns);
        Serial.print("\n"); 
        Serial.print("Longitude: ");
        Serial.print(lon, 7);
        Serial.print("° ");
        Serial.print(ew);
        Serial.print("\n"); 
        Serial.print("Time: ");
        Serial.print(hour);
        Serial.print(":");
        Serial.print(minute);
        Serial.print(":");
        Serial.print(second);
        Serial.println(" UTC");
    } else {
        Serial.println("No valid GPS fix from GPRMC! Waiting...");
    }
}