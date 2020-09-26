#include <Arduino.h>
#include <WiFi.h>
#include <MQTT.h>

#include <constants.h>
#include <connect.h>

WiFiClient net;
MQTTClient client;


bool led_status = false;
long reportTimer = millis();


void inputHandler(String &topic, String &payload) {
  if(topic == "esp32/led"){
    led_status == !led_status;
    digitalWrite(13, led_status);
    Serial.println(payload);
  }
}

void setup() {
  pinMode(13, OUTPUT);

  Serial.begin(115200);
  WiFi.begin(SSID, SSID_PASSWORD);

  client.begin(BROKER, net);
  client.onMessage(inputHandler);

  connect(Serial, net, client);
}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect(Serial, net, client);
  }

  if(millis()-reportTimer >= REPORT_FREQ){
    reportTimer=millis();
    client.publish(String(MQTT_ID)+"/report", String(analogRead(A0)));
  }

}