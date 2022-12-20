#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <SPI.h>                  //Einbinden von SPI für Kommunikation  mit DOT Matrix
#include <Adafruit_GFX.h>         //Zeichensätze für DOT Matrix
#include <Max72xxPanel.h>         //Ansteuerung für DOT Matrix mit MAX7219 Treiber

#define wifi_ssid "SSID-HERE"
#define wifi_password "PASSWORD-HERE"

int pinCS = 0;                          //CS Pin Select
int numberOfHorizontalDisplays = 4;     //Anzahl der Module Horizontal
int numberOfVerticalDisplays = 1;       //Anzahl der Module Vertikal

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

String LaufschriftText = "Loading ...";  //Default Laufschrift

int wait = 50;                  //Zeit in ms für Scroll Geschwindigkeit wo gewartet wird
int helligkeit = 1;             //Default Helligkeit 0 bis 15
int spacer = 1;                 //leer Zeichen länge
int width = 5 + spacer;         //Schriftlänge ist 5 Pixel

WiFiClient espClient;


void setup() {
  delay(1000);                  //1 Sekunde warten

  matrix.setIntensity(helligkeit); //Helligkeit von Display auf Default Wert einstellen

  matrix.setRotation(0, 1);        //Erste DOT Matrix Drehen
  matrix.setRotation(1, 1);        //Zweite DOT Matrix Drehen
  matrix.setRotation(2, 1);        //Dritte DOT Matrix Drehen
  matrix.setRotation(3, 1);        //Vierte DOT Matrix Drehen

  Serial.begin(9600);
  setup_wifi();
}


String http_request_temp_humi(String request_url1, String request_url2, String description, String format1, String format2) {
    
    HTTPClient http;
    
    http.begin(request_url1); 
    int httpCode1 = http.GET();
    String payload1 = http.getString();   
    Serial.println(payload1);
    http.end();

    http.begin(request_url2); 
    int httpCode2 = http.GET();
    String payload2 = http.getString();   
    Serial.println(payload2);
    http.end();
    
    LaufschriftText = description + payload1 + format1 + " / " + payload2 + format2;
    
    return LaufschriftText;
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi OK ");
  Serial.print("=> ESP8266 IP address: ");
  Serial.print(WiFi.localIP());
}

// Loading Animation Screen -------------------------------------------------------------------

void loading_screen() {
    int inc = -2;
    for (int i = 0; i <= 5; i++) {
      for ( int x = 0; x < matrix.width() - 1; x++ ) {
        matrix.fillScreen(LOW);
        matrix.drawLine(x, 0, matrix.width() - 1 - x, matrix.height() - 1, HIGH);
        matrix.write(); // Send bitmap to display
        delay(wait);
      }

      for ( int y = 0; y < matrix.height() - 1; y++ ) {
        matrix.fillScreen(LOW);
        matrix.drawLine(matrix.width() - 1, y, 0, matrix.height() - 1 - y, HIGH);
        matrix.write(); // Send bitmap to display
        delay(wait);
      }

      Serial.println(i);
      }

}

void show() {
  for ( int i = 0 ; i < width * LaufschriftText.length() + matrix.width() - 1 - spacer; i++ ) {

    matrix.fillScreen(LOW);

    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; //Zentrieren des Textes Vertikal

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < LaufschriftText.length() ) {
        matrix.drawChar(x, y, LaufschriftText[letter], HIGH, LOW, 1);
        
      }
      letter--;
      x -= width;
    }

    matrix.write(); //Display beschreiben
    delay(wait);    //Warten für Scroll Geschwindigkeit
  }
}


void loop() {
  show();
  http_request_temp_humi("http://openhabianpi:8080/rest/items/MQTT_Tempsensor_Office_Temp/state", "http://openhabianpi:8080/rest/items/MQTT_Tempsensor_Office_Humi/state", "Office: ", "C", "%");
  show();
  http_request_temp_humi("http://openhabianpi:8080/rest/items/temperatureSQL/state", "http://openhabianpi:8080/rest/items/humiditySQL/state", "Outdoor: ", "C", "%");
  

}
