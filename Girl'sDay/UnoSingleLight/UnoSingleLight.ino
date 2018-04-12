#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#define TRIG_PIN 2
#define SENSOR_PIN 4

int trigNow = LOW;
int trigTo = LOW;
int sensorSwitch = 1;

void setup(){
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"gdsgl");   // 本地名
  Mirf.payload = 5;   // 长度
  Mirf.channel = 3;   // 信道
  Mirf.config();
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(SENSOR_PIN, INPUT);
}

void loop(){
  if (sensorSwitch==1){
    int sensorNow=digitalRead(SENSOR_PIN);
    if (sensorNow==HIGH) trigTo=HIGH;
  }
  if (trigNow != trigTo){
    digitalWrite(TRIG_PIN, trigTo);
    trigNow=trigTo;
  }
  String RecieveStr=Recieve();
  if (RecieveStr=="swto1") trigTo=HIGH;
  if (RecieveStr=="swto0") trigTo=LOW;
  if (RecieveStr=="seto1") sensorSwitch=HIGH;
  if (RecieveStr=="seto0") sensorSwitch=LOW;
  delay(200);
}

String Recieve(){
  String Temp="";
  if (!Mirf.isSending() && Mirf.dataReady()){   // 存在数据
    byte data[Mirf.payload];
    Mirf.getData(data);
    for (int i=0;i<Mirf.payload;i++) Temp+=char(data[i]);
  }
  return Temp;
}

