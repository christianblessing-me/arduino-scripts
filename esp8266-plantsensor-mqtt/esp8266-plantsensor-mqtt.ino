#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
   
#define wifi_ssid "SSID-HERE"
#define wifi_password "PASSWORD-HERE"

#define mqtt_server "MQTT-SERVER-IP"
#define mqtt_user "MQTT-USER"     
#define mqtt_password "MQTT-PASSWORD"  

#define plant_humidity_topic "plantsensor01/humidity"
#define plant_value_topic "plantsensor01/value"

char message_buff[100];
long lastMsg = 0;   
long lastRecu = 0;
bool debug = false;  

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  setup_wifi();          
  client.setServer(mqtt_server, 1883);    
  client.setCallback(callback);      
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

void reconnect() {

  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker ...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("OK");
    } else {
      Serial.print("KO, error : ");
      Serial.print(client.state());
      Serial.println(" Wait 5 secondes before to retry");
      delay(10000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  int val=analogRead(A0); 

  if (val > 250 && val < 499) {
    Serial.print("Feuchtigkeit: ");
    Serial.println("100.00%");
    float t = 100;
    client.publish(plant_humidity_topic, String(t).c_str(), true);
  }
  if (val > 500 && val < 599) {
    Serial.print("Feuchtigkeit: ");
    Serial.println("80.00%");
    float t = 80;
    client.publish(plant_humidity_topic, String(t).c_str(), true);
  }
  if (val > 600 && val < 699) {
    Serial.print("Feuchtigkeit: ");
    Serial.println("60.00%");
    float t = 60;
    client.publish(plant_humidity_topic, String(t).c_str(), true);
  }
  if (val > 700 && val < 799) {
    Serial.print("Feuchtigkeit: ");
    Serial.println("40.00%");
    float t = 40;
    client.publish(plant_humidity_topic, String(t).c_str(), true);
  }
  if (val > 800 && val < 899) {
    Serial.print("Feuchtigkeit: ");
    Serial.println("20.00%");
    float t = 20;
    client.publish(plant_humidity_topic, String(t).c_str(), true);
  }
   if (val > 900) {
    Serial.print("Feuchtigkeit: ");
    Serial.println("0.00%");
    float t = 0;
    client.publish(plant_humidity_topic, String(t).c_str(), true);
  }
  client.publish(plant_value_topic, String(t).c_str(), true);
  delay(10000);
}

void callback(char* topic, byte* payload, unsigned int length) {

  int i = 0;

  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
  String msgString = String(message_buff);

}