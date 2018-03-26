#include <Wire.h>

// A1332 I2C address is 0x0C(12)
#define Addr 0x0C

void setup()
{
  // Initialise I2C communication as Master
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);
  delay(300);
}

void loop()
{
  unsigned int data[4];

  // Start I2C Transmission 
  Wire.beginTransmission(Addr);
  // Stop I2C Transmission 
  Wire.endTransmission();

  // Request 2 byte of data
  Wire.requestFrom(Addr, 2);
  
  // Read 2 bytes of data
  // raw_adc msb, raw_adc lsb 

  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Checking valid data
  while((data[0] == 0) && (data[1] == 0))
  {
    // Request 2 byte of data
    Wire.requestFrom(Addr, 2);
  
    // Read 2 bytes of data
    // raw_adc msb, raw_adc lsb 
    if(Wire.available() == 2)
    {
      data[0] = Wire.read();
      data[1] = Wire.read();
    }
  }
  
  // Convert the data to 12-bits
  int raw_adc = ((data[0] & 0x0F) * 256) + (data[1] & 0xFF);
  float angle = (raw_adc * 360.0) / 4096.0;

  delay(500);

  if(Wire.available() == 2)
  {
    data[3] = Wire.read();
    data[4] = Wire.read();
  }

  // Checking valid data
  while((data[3] == 0) && (data[4] == 0))
  {
    // Request 2 byte of data
    Wire.requestFrom(Addr, 2);
  
    // Read 2 bytes of data
    // raw_adc msb, raw_adc lsb 
    if(Wire.available() == 2)
    {
      data[3] = Wire.read();
      data[4] = Wire.read();
    }
  }
  
  // Convert the data to 12-bits
  int raw_adc2 = ((data[3] & 0x0F) * 256) + (data[4] & 0xFF);
  float angle2 = (raw_adc2 * 360.0) / 4096.0;

  if(angle2 < angle)
    angle2 = 360.0 + angle2;
  float rpm = (angle2 - angle) / 360.0 / 0.5 * 60.0;

  // Serial.print("rpm: ");
  Serial.println(rpm);
  delay(500);
}

