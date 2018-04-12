// TEMP: ds18b20
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4 // D4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// HUMI: dht11
#include <dht11.h>
#define DHT11PIN 6
dht11 dht11;

// PRES: bmp180
#include <Wire.h>
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;

String float2str(float target,int dpLen){
  char buffer[16];
  dtostrf(target,0,dpLen,buffer);
  return buffer;
}

void setup(){
  Serial.begin(9600);
  
  delay(3000);
  
  // TEMP: ds18b20
  sensors.begin();
  
  // PRES: bmp180
  bmp.begin();
}

void doUpdate(){
  sensors.requestTemperatures();
  dht11.read(DHT11PIN);
  
  // TEMP: ds18b20
  delay(500);
  String TEMP = float2str(sensors.getTempCByIndex(0),2);
  
  // HUMI: dht11
  String HUMI = float2str(dht11.humidity,2);
  
  // PRES: bmp180
  String PRES = float2str(float(bmp.readPressure())/1000,2);
  String ALTI = float2str(bmp.readAltitude(),2);

  Serial.print("Temperature: "+TEMP);
  Serial.print("   Humidity: "+HUMI);
  Serial.print("   Pressure: "+PRES);
  Serial.println("   Altitude: "+ALTI);
}

void loop(){
  doUpdate();
  delay(2500);
}

