int n=250;
int delayTime=100;
int num=0;

void setup(){
  Serial.begin(9600);
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  flow();
}

void loop(){

}

void flow(){
  clickEvent();
  light(10);
  clickEvent();
  light(9);
  clickEvent();
  light(6);
  clickEvent();
  light(5);
  clickEvent();
  light(3);
  flow();
}

void clickEvent(){
  int up=digitalRead(8);
  int down=digitalRead(7);
  if (up==HIGH){
    if ((n+25)<=250) n=n+25;
  }
  if (down==HIGH){
    if ((n-25)>=0) n=n-25;
  }
}

void light(int lightNum){
  analogWrite(lightNum,n);
  num=lightNum;
  delay(delayTime);
  analogWrite(lightNum,0);
}
