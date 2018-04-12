#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

const char *ssid = "GirlsDay";
const char *password = "123456";

ESP8266WebServer server(80);

void handleIndex(){
  String message = "<h2>Girl's Day Controller</h2>\n\n";
  server.send(200, "text/html", message);
}

void handleGDELWswto1(){
  Mirf.setTADDR((byte *)"gdelw");
  Send("swto1");
  String message = "<h3>Command Sent.</h3>\n\n";
  server.send(200, "text/html", message);
}

void handleGDELWswto0(){
  Mirf.setTADDR((byte *)"gdelw");
  Send("swto0");
  String message = "<h3>Command Sent.</h3>\n\n";
  server.send(200, "text/html", message);
}

void handleGDDBLswto1(){
  Mirf.setTADDR((byte *)"gddbl");
  Send("swto1");
  String message = "<h3>Command Sent.</h3>\n\n";
  server.send(200, "text/html", message);
}

void handleGDDBLswto0(){
  Mirf.setTADDR((byte *)"gddbl");
  Send("swto0");
  String message = "<h3>Command Sent.</h3>\n\n";
  server.send(200, "text/html", message);
}

void handleGDDBLseto1(){
  Mirf.setTADDR((byte *)"gddbl");
  Send("seto1");
  String message = "<h3>Command Sent.</h3>\n\n";
  server.send(200, "text/html", message);
}

void handleGDDBLseto0(){
  Mirf.setTADDR((byte *)"gddbl");
  Send("seto0");
  String message = "<h3>Command Sent.</h3>\n\n";
  server.send(200, "text/html", message);
}

void handleGDSGLswto1(){
  Mirf.setTADDR((byte *)"gdsgl");
  Send("swto1");
  String message = "<h3>Command Sent.</h3>\n\n";
  server.send(200, "text/html", message);
}

void handleGDSGLswto0(){
  Mirf.setTADDR((byte *)"gdsgl");
  Send("swto0");
  String message = "<h3>Command Sent.</h3>\n\n";
  server.send(200, "text/html", message);
}

void handleGDSGLseto1(){
  Mirf.setTADDR((byte *)"gdsgl");
  Send("seto1");
  String message = "<h3>Command Sent.</h3>\n\n";
  server.send(200, "text/html", message);
}

void handleGDSGLseto0(){
  Mirf.setTADDR((byte *)"gdsgl");
  Send("seto0");
  String message = "<h3>Command Sent.</h3>\n\n";
  server.send(200, "text/html", message);
}

void handleNotFound(){
  String message = "<h3>File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "<br>\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "<br>\nArguments: ";
  message += server.args();
  message += "<br>\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "<br>\n";
  }
  message += "</h3>";
  server.send(404, "text/html", message);
}

void setup(){
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Mirf.cePin = 5;
  Mirf.csnPin = 4;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"gdctl");   // 本地名
  Mirf.payload = 5;   // 长度
  Mirf.channel = 3;   // 信道
  Mirf.config();

  server.on("/", handleIndex);
  server.on("/gdelw/swto1", handleGDELWswto1);
  server.on("/gdelw/swto0", handleGDELWswto0);
  server.on("/gddbl/swto1", handleGDDBLswto1);
  server.on("/gddbl/swto0", handleGDDBLswto0);
  server.on("/gddbl/seto1", handleGDDBLseto1);
  server.on("/gddbl/seto0", handleGDDBLseto0);
  server.on("/gdsgl/swto1", handleGDSGLswto1);
  server.on("/gdsgl/swto0", handleGDSGLswto0);
  server.on("/gdsgl/seto1", handleGDSGLseto1);
  server.on("/gdsgl/seto0", handleGDSGLseto0);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop(){
  server.handleClient();
}

void Send(char *str){
  int lens=strlen(str);
  char msg[lens];
  for (int i=0;i<lens;i++) msg[i]= int(str[i]);
  Mirf.send((byte *)&msg);
  while(Mirf.isSending()) delay(1);
}

