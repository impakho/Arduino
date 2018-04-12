void setup(){
  
}

void loop(){
  for(int i=200;i<=800;i++){
    pinMode(7,OUTPUT);
    tone(7,i);
    delay(5);
  }
  delay(4000);
  for(int i=800;i>=200;i--){
    pinMode(7,OUTPUT);
    tone(7,i);
    delay(10);
  }
}
