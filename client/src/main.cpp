#include "Arduino.h"
#include "WiFi.h"
#include "PubSubClient.h"
#include "RTC_.h"
#include "SparkFunMPL3115A2.h"
#include <Esp.h>
#define ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD 1e11
#include <ArduinoJson.h>
#include "DHT.h"
#include <iostream> // std::cout
#include <string>


#define WIFI_SSID "1103"
#define WIFI_PASSWORD "luiz2511"
#define MQTT_HOST "storni.serveblog.net"
#define MQTT_PORT 5883

#define DHTPIN 15
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient mqtt(espClient);

String mqttName = "AEGIS";
String dataTopic = "ARGOS/data";

void callback(char *topic, byte *payload, unsigned int lenght);
void readPluviometro();
String getJSON();

RTC rtc;
hw_timer_t * timer = NULL;
int rebootTime = 15; //segundos
MPL3115A2 baro;

#define PlUVI 14
int val = 0;
int old_val = 0;
int pulse = 0;

double timestamp = 0;
int printTime = 30;

void setup()
{
  Serial.begin(9600);
  delay(500);
  dht.begin();
  Wire.begin();
  baro.begin();
  baro.setModeBarometer();
  baro.setOversampleRate(7);
  baro.enableEventFlags();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("Connecting to WiFi: ");
    Serial.println(WIFI_SSID);
  }
  Serial.println("Connected to the WiFi network\nI address: " + String(WiFi.localIP()));
  mqtt.setServer(MQTT_HOST, MQTT_PORT);

  while (!mqtt.connected())
  {
    Serial.println("Connecting to MQTT...");
    if (mqtt.connect(mqttName.c_str()))
      Serial.println("Connected");
    else
    {
      Serial.print("Failed with state ");
      Serial.println(mqtt.state());
    }
  }
  mqtt.setCallback(callback);
  rtc.askUnix(mqtt);
  delay(1000);
}

void callback(char *topic, byte *payload, unsigned int lenght)
{
  rtc.setTimeMQTT(mqtt, topic, payload, lenght);
  Serial.println("Recebi uma mensagem!");
}

void loop()
{
  if (!mqtt.connected())
    mqtt.connect(mqttName.c_str());

  double now = rtc.getTime();
  if ((now - timestamp) > printTime)
  {
    timestamp = now;
    String out = getJSON();
    Serial.println(out);
    mqtt.publish(dataTopic.c_str(), out.c_str(), false);
  }
  readPluviometro();
  mqtt.loop();
  delay(1);
}

String getJSON()
{
  StaticJsonDocument<164> doc;
  doc["sensor"] = mqttName;
  doc["temperature"] = dht.readTemperature();
  doc["humidity"] = dht.readHumidity();
  doc["pluviometric"] =pulse*0.25;
  doc["pressure"] = baro.readPressure()/100;
  doc["temperature_2"] = baro.readTemp();
  pulse = 0;
  doc["timeTX"] = String(rtc.getTime(), 3);
  return doc.as<String>();
}

void readPluviometro(){
  val = digitalRead(PlUVI); 

  if ((val == LOW) && (old_val == HIGH)){            
    delay(10); 
    pulse++;
    old_val = val;
  }
  else{
    old_val = val;
  }
}
