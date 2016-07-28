// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TSL26711
// This code is designed to work with the TSL26711_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/products

#include <application.h>
#include <spark_wiring_i2c.h>

// I2C address of the TSL26711 0x39(57)
#define Addr 0x39

int proximity = 0;
void setup()
{
  // Set variable
  Particle.variable("i2cdevice", "TSL26711");
  Particle.variable("proximity", proximity);

  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select enable register
  Wire.write(0x00 | 0x80);
  // Power on, Wait enabled, Proximity enabled
  Wire.write(0x0F);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select proximity time control register
  Wire.write(0x02 | 0x80);
  // time = 2.72 ms
  Wire.write(0xFF);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select wait time control register
  Wire.write(0x03 | 0x80);
  // time = 2.72 ms
  Wire.write(0xFF);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select pulse count register
  Wire.write(0x0E | 0x80);
  // Pulse count = 32
  Wire.write(0x20);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register
  Wire.write(0x0F | 0x80);
  // Proximity uses Ch1 diode
  Wire.write(0x20);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  unsigned int data[0];

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x18 | 0x80);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data
  // proximity lsb, proximity msb
  if (Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data
  int proximity = data[1] * 256 + data[0];

  // Output data to dashboard
  Particle.publish("Proximity Of The Device : ", String(proximity));
  delay(1000);
}
