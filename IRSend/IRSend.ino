#include <IRremote.h>
 
IRsend irsend;
unsigned int buf[5]=
    {8300,4150,550,600,500};
 
void setup()
{
}
 
void loop() {
  irsend.sendRaw(buf,5,38);
  delay(500);
}
