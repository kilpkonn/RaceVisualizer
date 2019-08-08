#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <SPI.h>
#include <SD.h>

#define SD_CARD_CS 8
#define SD_CARD_MOSI 11
#define SD_CARD_CLK 13
#define SD_CARD_MISO 12

#define GPS_RX 4
#define GPS_TX 3

TinyGPS gps;
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
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and save some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (gpsSerial.available())
    {
      char c = gpsSerial.read();
      // Serial.write(c);
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }
  if (newData) {
    float flat, flon;
    unsigned long age, date, time;
    gps.f_get_position(&flat, &flon, &age);
    gps.get_datetime(&date, &time, &age);

    trackFile = SD.open("track.csv", FILE_WRITE);
      if (trackFile) {
        trackFile.print(date == TinyGPS::GPS_INVALID_DATE ? 0.0 : date);
        trackFile.print(',');
        trackFile.print(time == TinyGPS::GPS_INVALID_TIME ? 0.0 : time);
        trackFile.print(',');
        trackFile.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
        trackFile.print(',');
        trackFile.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
        trackFile.print(',');
        trackFile.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
        trackFile.print(',');
        trackFile.print(gps.f_speed_knots() == TinyGPS::GPS_INVALID_F_SPEED ? 0 : gps.f_speed_knots());
        trackFile.print(',');
        trackFile.print(gps.f_course() == TinyGPS::GPS_INVALID_F_ANGLE ? 0 : gps.f_course());
        trackFile.print(',');
        trackFile.print(gps.altitude() == TinyGPS::GPS_INVALID_ALTITUDE ? 0 : gps.altitude());
        trackFile.print(',');
        trackFile.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
        trackFile.print(',');
        trackFile.close();
      }
  }
}
