#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* IP_MQTT = "4.228.225.124";

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(5, DHT11);

void connectWiFi(){
  if (WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, password);
    if (WiFi.status() == WL_CONNECTED){
      Serial.println("Dispositivo conectado ao wifi");
    }
  }
}

void setup(){
  pinMode(33, INPUT);
  connectWiFi();
  client.setServer(IP_MQTT, 1883);
  dht.begin();
  Serial.begin(9600);

}

void loop(){

  if (!client.connected()){
    client.connect("device005");
    if (client.connected()){
      Serial.println("Dispositivo conectado ao Broker");
    }
  }
  int temperatura = dht.readTemperature();
  int umidade = dht.readHumidity();
  int luminosidade = analogRead(33);
  luminosidade = map(luminosidade, 0, 4000, 0, 100);
  Serial.println("luminosidade: ");
  Serial.println(luminosidade);
  Serial.print("%");
  Serial.println("temperatura: ");
  Serial.println(temperatura);
  Serial.println("umidade: ");
  Serial.println(umidade);

  String temperatura_m = String(temperatura);
  String luminosidade_m = String(luminosidade);
  String umidade_m = String(umidade);
  client.publish("/TEF/device005/attrs/temperatura", temperatura_m.c_str());
  client.publish("/TEF/device005/attrs/umidade", umidade_m.c_str());
  client.publish("/TEF/device005/attrs/luminosidade", luminosidade_m.c_str());
  delay(1000);

  client.loop();
}

