#include <SoftwareSerial.h>

#define WIFI_SSID       "SSID"
#define WIFI_PASSWORD   "123456"

SoftwareSerial wifiSerial(8,10);

String int2str(int target){
  char str[10];
  itoa(target,str,10);
  return str;
}

void setup(){
  pinMode(13,OUTPUT);
  beginSerial();
  if (!wifi_Status()) while(1);
  if (!wifi_Version()) while(1);
  if (!wifi_SetMode(1)) while(1);
  if (!wifi_JoinAP(WIFI_SSID,WIFI_PASSWORD)) while(1);
  if (!wifi_SetMUX(1)) while(1);
  wifi_StartTCP("8998");
}

void loop(){
  String data = "";
  char a;
  while(wifiSerial.available() > 0) {
    a = wifiSerial.read();
    if(a == '\0') continue;
    data += a;
  }
  if (data.indexOf("+IPD") != -1) {
    data+="END";
    String mData;
    String beginStr=":";
    String endStr="END";
    int index1 = data.indexOf(beginStr);
    int index2 = data.indexOf(endStr);
    if (index1 != -1 && index2 != -1) {
      index1 += beginStr.length();
      mData = data.substring(index1, index2);
    }
    wifiSerial_send("AT+CIPCLOSE=0",1);
    if (mData.indexOf("OPEN13") != -1) digitalWrite(13,HIGH);
    if (mData.indexOf("CLOSE13") != -1) digitalWrite(13,LOW);
    delay(200);
  }
}

void beginSerial() {
  Serial.begin(9600);
  Serial.println("esp8266 demo\r\n");
  wifiSerial.begin(9600);
}

boolean wifi_Status() {
  wifiSerial_send("AT",1);
  if (recv_find("OK",3000)) {
    Serial.println("esp8266 connect ok");
    return true;
  }
  Serial.println("esp8266 connect err");
  return false;
}

boolean wifi_Version() {
  wifiSerial_send("AT+GMR",1);
  String data;
  if (recv_filter("OK","\r\r\n","\r\nOK",data,3000)) {
    Serial.print(data);
    return true;
  }
  Serial.println("get version err");
  return false;
}

boolean wifi_SetMode(int mode) {
  wifiSerial_send("AT+CWMODE="+int2str(mode),1);
  String data;
  if (mode==1) data="Station";
  if (mode==2) data="SoftAP";
  if (mode==3) data="Station + SoftAP";
  if (recv_find("OK",3000)) {
    Serial.println("SetMode: \""+data+"\" succ");
    return true;
  }
  Serial.println("SetMode: \""+data+"\" err");
  return false;
}

boolean wifi_JoinAP(String target1, String target2) {
  wifiSerial_send("AT+CWJAP=\""+target1+"\",\""+target2+"\"",1);
  if (recv_find("OK","No AP",10000)) {
    wifiSerial_send("AT+CIFSR",1);
    String data;
    if (recv_filter("OK","\r\r\n","\r\n\r\nOK",data,3000)){
      Serial.println("JoinAP \""+target1+"\" succ");
      Serial.println(data);
      return true;
    }
  }
  Serial.println("JoinAP \""+target1+"\" err");
  return false;
}

boolean wifi_SetMUX(int mux) {
  wifiSerial_send("AT+CIPMUX="+int2str(mux),1);
  String data;
  if (mux==0) data="Single";
  if (mux==1) data="Mulitple";
  if (recv_find("OK",3000)) {
    Serial.println("SetMUX: \""+data+"\" succ");
    return true;
  }
  Serial.println("SetMUX: \""+data+"\" err");
  return false;
}

boolean wifi_StartTCP(String target1) {
  wifiSerial_send("AT+CIPSERVER=1,"+target1,1);
  if (recv_find("OK","no change",5000)) {
    Serial.println("StartTCP: \""+target1+"\" succ");
    return true;
  }
  Serial.println("StartTCP: \""+target1+"\" err");
  return false;
}

String wifi_Send(String target1, String target2, int target3) {
  String sendStr = "GET "+target2+" HTTP/1.1\r\n";
  sendStr += "Host: "+target1+"\r\n";
  sendStr += "Connection: close\r\n\r\n";
  wifiSerial_send("AT+CIPSEND="+int2str(sendStr.length()),1);
  delay(50);
  wifiSerial_send(sendStr,0);
  Serial.print("GET "+target1+" "+target2+" ...");
  String data="";
  if (target3==1) {
    if (wifiSerial_recvGET(data,10000)) {
      Serial.println("succ");
      return data;
    }
    Serial.print("err");
    return data;
  }else{
    Serial.print("succ");
    return data;
  }
}

boolean recv_find(String target, int timeout) {
  String data_tmp;
  data_tmp = wifiSerial_recv(target, timeout);
  if (data_tmp.indexOf(target) != -1) return true;
  return false;
}

boolean recv_find(String target1, String target2, int timeout) {
  String data_tmp;
  data_tmp = wifiSerial_recv(target1, target2, timeout);
  if (data_tmp.indexOf(target1) != -1 || data_tmp.indexOf(target2) != -1) return true;
  return false;
}

boolean recv_filter(String target, String beginStr, String endStr, String &data, int timeout) {
  String data_tmp;
  data_tmp = wifiSerial_recv(target, timeout);
  if (data_tmp.indexOf(target) != -1) {
    int index1 = data_tmp.indexOf(beginStr);
    int index2 = data_tmp.indexOf(endStr);
    if (index1 != -1 && index2 != -1) {
      index1 += beginStr.length();
      data = data_tmp.substring(index1, index2);
      return true;
    }
  }
  data = data_tmp;
  return false;
}

void wifiSerial_send(String target, int withln) {
  if (withln==1) wifiSerial.println(target);
  if (withln==0) wifiSerial.print(target);
}

String wifiSerial_recv(String target, int timeout) {
  String data;
  char a;
  unsigned long start = millis();
  while (millis() - start < timeout) {
    while(wifiSerial.available() > 0) {
      a = wifiSerial.read();
      if(a == '\0') continue;
      data += a;
    }
    if (data.indexOf(target) != -1) break;
  }
  return data;
}

String wifiSerial_recv(String target1, String target2, int timeout) {
  String data;
  char a;
  unsigned long start = millis();
  while (millis() - start < timeout) {
    while(wifiSerial.available() > 0) {
      a = wifiSerial.read();
      if(a == '\0') continue;
      data += a;
    }
    if (data.indexOf(target1) != -1 || data.indexOf(target2) != -1) break;
  }
  return data;
}

boolean wifiSerial_recvGET(String &mData, int timeout) {
  String beginStr="SEND OK";
  String middleStr=":";
  String endStr="CLOSED";
  String data;
  unsigned long start = millis();
  while (millis() - start < timeout) {
    while (wifiSerial.available() > 0) {
      data += char(wifiSerial.read());
    }
    if (data.indexOf(endStr) != -1) {
      int index1 = data.indexOf(beginStr);
      int index2 = data.indexOf(endStr);
      if (index1 != -1 && index2 != -1) {
        index1 += beginStr.length();
        data = data.substring(index1, index2)+endStr;
        int index3 = data.indexOf(middleStr);
        int index4 = data.indexOf(endStr);
        if (index3 != -1 && index4 != -1) {
          index3 += middleStr.length();
          mData = data.substring(index3, index4);
          return true;
        }
      }
    }
  }
  mData="";
  return false;
}
