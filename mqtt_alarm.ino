#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "ZAHRA"; // Enter your WiFi name
const char *password = "123456780";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "37.32.28.73";
const char *topic = "alarm/amin/5";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int mqtt_port = 1883;


WiFiClient espClient;
PubSubClient client(espClient);


// Sensor
int inputPin = D2;
unsigned long newTime = 10000;
unsigned long oldTime = 0;


void setup() {
  pinMode(inputPin, INPUT);
  Serial.begin(9600);
  delay(1000);
  
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp8266-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str())) {
          Serial.println("Public emqx mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
  // publish and subscribe
  client.publish(topic, "");
  
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void loop(){
  client.loop();
  
  int val = digitalRead(inputPin);
  if (val == HIGH) {
    newTime = millis();
    if ((oldTime + 10000) < newTime){
      Serial.println("Motion detected!");
      client.publish(topic, "");
      oldTime = millis();
    }
  }
  
  delay(1000);
}
