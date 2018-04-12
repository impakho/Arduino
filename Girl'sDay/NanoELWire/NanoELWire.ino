#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#define TRIG_PIN 2

int trigNow = LOW;
int trigTo = HIGH;

void setup(){
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"gdelw");   // 本地名
  Mirf.payload = 5;   // 长度
  Mirf.channel = 3;   // 信道
  Mirf.config();
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
}

void loop(){
  if (trigNow != trigTo){
    digitalWrite(TRIG_PIN, trigTo);
    trigNow=trigTo;
  }
  String RecieveStr=Recieve();
  if (RecieveStr=="swto1") trigTo=HIGH;
  if (RecieveStr=="swto0") trigTo=LOW;
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

