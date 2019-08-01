#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

#define SD_CARD_CS 8
#define SD_CARD_MOSI 11
#define SD_CARD_CLK 13
#define SD_CARD_MISO 12

#define GPS_RX 4
#define GPS_TX 3

File trackFile;
SoftwareSerial gpsSerial(GPS_TX, GPS_RX); // RX, TX

void setup() {
    //Serial.begin(9600);
    //Serial.println("Initializing SD card...");
    while (!SD.begin(SD_CARD_CS)) {
        delay(30 * 1000);
    }
    //Serial.println("SD Card ready!");
    gpsSerial.begin(9600);
    //Serial.println("GPS ready!");
}

void loop() {
    if (gpsSerial.available()) {
        char c = gpsSerial.read();
        trackFile = SD.open("track.txt", FILE_WRITE);
        if (trackFile) {
            trackFile.print(c);
            trackFile.close();
        }
    }
}
