int BAT = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int value = LOW;
  float Tvoltage=0.0;
  float Vvalue=0.0,Rvalue=0.0;
  
  for(unsigned int i=0;i<10;i++){
    Vvalue=Vvalue+analogRead(BAT);
    delay(5);
  }
  
  Vvalue=(float)Vvalue/10.0;            
  Rvalue=(float)(Vvalue/1024.0)*3.3;      
  Tvoltage=Rvalue*1.27;
  value = HIGH;
  Serial.println(Tvoltage);
}
