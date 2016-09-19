#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "env.h"

WiFiClient espClient;
PubSubClient client(espClient);

byte leds[] = {12, 13};
short blinkDelay = 500;
long ledSwitchTime = 0;
bool prOpen = false;
bool led1On = false;

void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == '1') {
    prOpen = true;
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), USER, MQ_PASS)) {
      Serial.println("connected");
      client.subscribe("pr");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      delay(5000);
    }
  }
}

void dismissMessage() {
  if (digitalRead(14) == HIGH) {
    digitalWrite(leds[0], LOW);
    digitalWrite(leds[1], LOW);
    prOpen = false;
  }
}

void alertPr() {
  if (prOpen) {
    if (millis() >= ledSwitchTime) {
      ledSwitchTime = millis() + blinkDelay;
      led1On = !led1On;

      if (led1On) {
        digitalWrite(leds[0], HIGH);
        digitalWrite(leds[1], LOW);
      } else {
        digitalWrite(leds[1], HIGH);
        digitalWrite(leds[0], LOW);
      }
    }
  }
}

void setup() {
  pinMode(leds[0], OUTPUT);
  pinMode(leds[1], OUTPUT);
  Serial.begin(115200);

  setup_wifi();
  client.setServer(SERVER, PORT);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  dismissMessage();
  alertPr();
}

