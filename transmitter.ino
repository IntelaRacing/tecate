#include <stdlib.h>
#include <SoftwareSerial.h> // needed for GPS and Radio
#include <Adafruit_GPS.h> // needed for GPS
#include <SPI.h>
#include "Adafruit_MAX31855.h"

#include <Wire.h>
#include "Adafruit_ADS1015.h"

Adafruit_ADS1015 ads1015(0x48);
Adafruit_ADS1115 ads1115(0x49);
Adafruit_ADS1115 ads1215(0x4A);

#define MAXDO0   35
#define MAXCS0   37
#define MAXCLK0  39
#define MAXDO1   34
#define MAXCS1   36
#define MAXCLK1  38
Adafruit_MAX31855 thermocouple1(MAXCLK0, MAXCS0, MAXDO0);
Adafruit_MAX31855 thermocouple2(MAXCLK1, MAXCS1, MAXDO1);


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

  ads1015.begin();
  ads1115.begin();
  ads1215.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(10);

  //int16_t adc00, adc01, adc02, adc03;
  //int16_t adc10, adc11, adc12, adc13;
  //int16_t adc20, adc21, adc22, adc23;
  double adc00 = int(ads1015.readADC_SingleEnded(0)/16.5);
  double adc10 = int(ads1115.readADC_SingleEnded(0)/264.0);
  double adc20 = int(ads1215.readADC_SingleEnded(0)/264.0);
  
  double c0 = thermocouple1.readCelsius();
  double f0 = (9.0/5.0)*c0+32.0;
  String sender1 = "-a+" + String(f0, 2) + "#";
  xbee.println(sender1);
  Serial.println(sender1);

  double c1 = thermocouple2.readCelsius();
  double f1 = (9.0/5.0)*c1+32.0;
  String sender11 = "-q+" + String(f1, 2) + "#";
  xbee.println(sender11);
  Serial.println(sender11);

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

  String sender5 = "-e+" + String(adc00,2) + "#";
  xbee.println(sender5);
  Serial.println(sender5);  

  String sender6 = "-f+" + String(adc10,2) + "#";
  xbee.println(sender6);
  Serial.println(sender6);

  String sender7 = "-g+" + String(adc20,2) + "#";
  xbee.println(sender7);
  Serial.println(sender7);

  int switchValue = analogRead(A0);
  String sender8 = "";
  if(switchValue > 60)
     sender8 = "-h+1#";
  else sender8 = "-h+0#";
  xbee.println(sender8);
  Serial.println(sender8);   
  

}