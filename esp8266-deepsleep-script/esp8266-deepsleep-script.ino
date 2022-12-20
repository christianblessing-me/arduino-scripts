void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2000);
  
  while(!Serial) { }
  
  Serial.println("I'm awake, but I'm going into deep sleep mode for 30 seconds");
  ESP.deepSleep(6e6);
  //ESP.deepSleep(6000);
  delay(100);
  
}

void loop() {
}
