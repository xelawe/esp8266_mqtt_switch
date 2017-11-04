#include <PubSubClient.h>

const char* mqtt_server = "mqtt.cytron.tiscali.at";
const char* mqtt_subtopic = "ATSH28/OG/Z1/RL/1/state";
//const char* mqtt_subtopic = "ATSH28/OG/Z1/SW/+/set";
const char* mqtt_pubtopic = "ATSH28/OG/Z1/SW/2/set";
char* mqtt_clientname;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void callback_mqtt(char* topic, byte* payload, unsigned int length) {
  DebugPrint("Message arrived [");
  DebugPrint(topic);
  DebugPrint("] ");
  for (int i = 0; i < length; i++) {
    DebugPrint((char)payload[i]);
  }
  DebugPrintln();

  // Switch on the LED if an 1 was received as first character
  switch ((char)payload[0]) {
    case '0':
      turnOff();
      break;
    case '1':
      turnOn();
      break;
    case '2':
      toggle();
      break;
  }
}

void reconnect_mqtt() {
  // Loop until we're reconnected
  // while (!client.connected()) {
  DebugPrint("Attempting MQTT connection...");
  // Attempt to connect
  if (client.connect(mqtt_clientname)) {
    DebugPrintln("connected");
    // Once connected, publish an announcement...
    //client.publish(mqtt_pubtopic, "hello world");
    // ... and resubscribe
    client.subscribe(mqtt_subtopic);
  } else {
    DebugPrint("failed, rc=");
    DebugPrintln(client.state());
    //DebugPrintln(" try again in 5 seconds");
    // Wait 5 seconds before retrying
    //delay(5000);
  }
  //}
}

void init_mqtt(char *iv_clientname) {
  mqtt_clientname = iv_clientname;
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback_mqtt);
}

void check_mqtt() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();
}

void pub_mqtt_toggle() {
  client.publish(mqtt_pubtopic, "2");
  DebugPrint(mqtt_pubtopic);
  DebugPrintln(" Message published");

}


