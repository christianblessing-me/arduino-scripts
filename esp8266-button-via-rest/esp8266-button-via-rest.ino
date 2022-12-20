#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
 
void setup() {
 
  Serial.begin(115200);                                  //Serial connection
  pinMode (D1, INPUT);
  pinMode (D2, OUTPUT);
  pinMode (D5, INPUT);
  WiFi.begin("SSID-HERE", "PASSWORD-HERE");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }
 
}
 
void loop() {
 
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status

  byte val1 = digitalRead(D1);
  byte val2 = digitalRead(D5);

   if (val1 == HIGH) {
      
      Serial.println("Val 1 HIGH!");

      HTTPClient http;    //Declare object of class HTTPClient
      http.begin("http://openhabianpi:8080/rest/items/DeskLed");      //Specify request destination
      http.addHeader("Content-Type", "text/plain");  //Specify content-type header
      int httpCode = http.POST("TOGGLE");   //Send the request
      String payload = http.getString();                  //Get the response payload
      Serial.println(httpCode);   //Print HTTP return code
      Serial.println(payload);    //Print request response payload
      http.end();  //Close connection
      delay(1000);
      //digitalWrite (D2, LOW);
    }
  if (val2 == HIGH) {
      HTTPClient http;  //Declare an object of class HTTPClient
 
      http.begin("http://openhabianpi:8080/rest/items/DeskLed/state");  //Specify request destination
      int httpCode = http.GET();                                                                  //Send the request
 
      if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
      if(payload == "ON") {
        digitalWrite (D2, HIGH);
      }
      else {
        digitalWrite (D2, LOW); 
      }
      
      http.end();   //Close connection
      
 
}
    }
  
   }
  }
