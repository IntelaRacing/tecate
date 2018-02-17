#include <stdlib.h>
#include <SoftwareSerial.h> // needed for GPS and Radio
#include <Adafruit_GPS.h> // needed for GPS
#include <SPI.h>
#include "Adafruit_MAX31855.h"

#define MAXDO   12
#define MAXCS   7
#define MAXCLK  13
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);


////// Radio //////
SoftwareSerial xbee(2,3); // RX, TX 
int pingPong = 1;

////// GPS //////
SoftwareSerial mySerial(6, 5); // TX,RX
Adafruit_GPS GPS(&mySerial);
#define GPSECHO true
boolean usingInterrupt(boolean);
int counter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.println("Testing TX/RX with Live GPS data!");
  Serial.println();

  xbee.begin(9600);
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  mySerial.println(PMTK_Q_RELEASE);


}

void loop() {
  // put your main code here, to run repeatedly:

  delay(100);
  
  double c = thermocouple.readCelsius();
  double f = (9.0/5.0)*c+32.0;
  String sender1 = "-a+" + String(f, 2) + "#";
  xbee.println(sender1);
  Serial.println(sender1);

  double lat = GPS.latitudeDegrees;
  String sender2 = "-b+" + String(lat, 2) + "#";
  xbee.println(sender2);
  Serial.println(sender2);

  double lon = GPS.longitudeDegrees;
  String sender3 = "-c+" + String(lon, 2) + "#";
  xbee.println(sender3);
  Serial.println(sender3);

  double speedy = 1.15078*GPS.speed;
  String sender4 = "-d+" + String(speedy,2) + "#";
  xbee.println(sender4);
  Serial.println(sender4);

  
}
