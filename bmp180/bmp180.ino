#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

void setup(){
  Serial.begin(9600);
  bmp.begin();
  Serial.println("bmp180 demo");
  delay(1000);
}

void loop(){
  Serial.print("Temperature: ");
  Serial.print(bmp.readTemperature());
  Serial.print(" C");
  Serial.print("   Pressure: ");
  Serial.print(float(bmp.readPressure())/1000);
  Serial.print(" kPa");
  Serial.print("   Altitude: ");
  Serial.print(bmp.readAltitude());
  Serial.print(" m");
  Serial.print("   Real Altitude: ");
  Serial.print(bmp.readAltitude(101500));
  Serial.println(" m");
  delay(2500);
}
