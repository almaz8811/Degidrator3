#include <FS.h>

#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp8266.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson

#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 2          // Назначить пин датчика температуры
#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//define your default values here, if there are different values in config.json, they are overwritten.
char blynk_token[34] = "620eaaca18f9458ba741da83bc2b4573";
const char *host = "esp8266-webupdate"; // Хост обновления
//flag for saving data
bool shouldSaveConfig = false;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
//callback notifying us of the need to save config
//void readData();

BlynkTimer timer;
DHT dht(DHTPIN, DHTTYPE);
void saveConfigCallback()
{
    Serial.println("Should save config");
    shouldSaveConfig = true;
}

int preset;
int time_sushki;
int rejim;
int temp_sushki;
int checkStart;
bool sbros = 0;
uint32_t myTimer;
float h;
float t;

BLYNK_CONNECTED()
{
    Blynk.syncAll();
}