#include <DHT.h>

DHT dht;

void setup(){
  Serial.begin(9600);
  dht.setup(3);
  Serial.println("DHT11 demo");
  delay(1000);
}

void loop(){
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
  Serial.print("Temperature is ");
  Serial.print(temperature, 1);
  Serial.println(" C");
  Serial.print("Humidity is ");
  Serial.print(humidity, 1);
  Serial.println("%");
  delay(2500);
}
