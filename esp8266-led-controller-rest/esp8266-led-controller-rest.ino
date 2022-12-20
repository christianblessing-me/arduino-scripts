#include <ESP8266WiFi.h>

const char* ssid = "SSID-HERE";
const char* password = "PASSWORD-HERE";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  digitalWrite(2, 0);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  server.begin();
  Serial.println("Server started");

  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  while(!client.available()){
    delay(1000);
  }
  
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  if (req.indexOf("/d1/on") > 0) digitalWrite (D1, 1);
  if (req.indexOf("/d1/off") > 0) digitalWrite (D1, 0);
  if (req.indexOf("/d2/on") > 0) digitalWrite (D2, 1);
  if (req.indexOf("/d2/off") > 0) digitalWrite (D2, 0);
  if (req.indexOf("/d3/on") > 0) digitalWrite (D3, 1);
  if (req.indexOf("/d3/off") > 0) digitalWrite (D3, 0);
  if (req.indexOf("/all/on") > 0) {
    digitalWrite (D1, 1);
    digitalWrite (D2, 1);
    digitalWrite (D3, 1);
    }
  if (req.indexOf("/all/off") > 0) {
    digitalWrite (D1, 0);
    digitalWrite (D2, 0);
    digitalWrite (D3, 0);
    }

  client.flush();

}
