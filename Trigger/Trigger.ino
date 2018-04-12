#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

int pinInOn=0;

void setup(){
  Mirf.cePin = 9;     // 设置CE引脚为D9
  Mirf.csnPin = 10;   // 设置CE引脚为D10
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"trig0");   // 本地名
  Mirf.payload = 5;   // 长度
  Mirf.channel = 3;   // 信道
  Mirf.config();
  pinMode(2,INPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  delay(3000);
}

void loop(){
  int pinIn=digitalRead(2);
  if (pinIn==HIGH&&pinInOn==0){
    Switch();
    pinInOn=1;
  }
  if (pinIn==LOW&&pinInOn==1) pinInOn=0;
  String RecieveStr=Recieve();
  if (RecieveStr!="") Serial.println(RecieveStr);
  if (RecieveStr=="swto1") SwitchOn();
  if (RecieveStr=="swto0") SwitchOff();
}

void Switch(){
  int pinOut=digitalRead(4);
  if (pinOut==LOW){
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
  }else{
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
  }
}

void SwitchOn(){
  int pinOut=digitalRead(4);
  if (pinOut==LOW){
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
  }
}

void SwitchOff(){
  int pinOut=digitalRead(4);
  if (pinOut==HIGH){
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
  }
}

String Recieve(){
  String Temp="";
  if (!Mirf.isSending() && Mirf.dataReady()){   // 存在数据
    byte data[Mirf.payload];
    Mirf.getData(data);
    for (int i=0;i<Mirf.payload;i++) Temp+=char(data[i]);
    /*Mirf.setTADDR((byte *)"gate0");   // 远程名
    Send("succ1");   // 发送数据*/
  }
  return Temp;
}

void Send(char *str){
  int lens=strlen(str);
  char msg[lens];
  for (int i=0;i<lens;i++) msg[i]= int(str[i]);
  Mirf.send((byte *)&msg);
  while(Mirf.isSending()){}
}
