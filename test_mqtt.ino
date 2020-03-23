#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define MAX_MSG_LEN (128)
 
const char* ssid = "Livebox-2ED0"; // Enter your WiFi name
const char* password =  "959E16E9DA1C23E5E421173C73"; // Enter WiFi password
const char* mqttServer = "192.168.1.24";
const int mqttPort = 1883;
const char* mqttUser = "edmonsoon";
const char* mqttPassword = "pi";

 int led= D0;
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
   pinMode(led, OUTPUT);
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("test", "salut ca va"); //Topic name
  client.subscribe("test");
 
}
void setLedState(boolean state) {
  // LED logic is inverted, low means on
  digitalWrite(A0, !state);
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  static char message[MAX_MSG_LEN+1];
  message[length] = '\0';
 
  strncpy(message, (char *)payload, length);
  
  Serial.println();
  Serial.println("-----------------------");

  if (strcmp(message, "off") == 0) {
    setLedState(false);
  } else if (strcmp(message, "on") == 0) {
    setLedState(true);
  }
}
 


 
void loop() {
  client.loop();
  


}
