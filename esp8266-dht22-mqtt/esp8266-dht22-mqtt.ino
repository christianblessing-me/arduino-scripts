#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
   
#define wifi_ssid "SSID-HERE"
#define wifi_password "PASSWORD-HERE"

#define mqtt_server "SERVER-IP"
#define mqtt_user "MQTT-USER"     
#define mqtt_password "MQTT-PASSWORD"  

#define temperature_topic "sensor02/temperature"  //Topic temperature
#define humidity_topic "sensor02/humidity"        //Topic humidity

//Buffer to decode MQTT messages
char message_buff[100];

long lastMsg = 0;   
long lastRecu = 0;
bool debug = false;  

#define DHTPIN            4         // Pin which is connected to the DHT sensor. 

#define DHTTYPE DHT22         


DHT dht(DHTPIN, DHTTYPE);     
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);        
  setup_wifi();          
  client.setServer(mqtt_server, 1883);    
  client.setCallback(callback);              
  dht.begin();
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
      delay(5000);
    }
  }
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(5000);

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    Serial.println(t);
    Serial.println(h);


  
      Serial.print("Temperature : ");
      Serial.print(t);
      Serial.print(" | Humidity : ");
      Serial.println(h);
    
    client.publish(temperature_topic, String(t).c_str(), true);   
    client.publish(humidity_topic, String(h).c_str(), true);      
  
}

void callback(char* topic, byte* payload, unsigned int length) {

  int i = 0;
  if ( debug ) {
    Serial.println("Message recu =>  topic: " + String(topic));
    Serial.print(" | longueur: " + String(length,DEC));
  }
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
  String msgString = String(message_buff);
  if ( debug ) {
    Serial.println("Payload: " + msgString);
  }

}