#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPUpdateServer.h>
#include <DNSServer.h>
#include <TickerScheduler.h> //https://github.com/Toshik/TickerScheduler
#include <DHT.h>             //https://github.com/markruys/arduino-DHT   Support for DHT11 and DHT22/AM2302/RHT03
#include <time.h>

// Объект для обнавления с web страницы
ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
ESP8266WebServer HTTP(80);

// Для файловой системы
File fsUploadFile;

// Для работы символьных имен в режиме AP
DNSServer dnsServer;

//Планировщик задач (Число задач)
TickerScheduler ts(2);

// Датчик DHT
DHT dht;
#define dhtPin 4

//String configSetup = "{}"; // данные для config.setup.json
//String configJson = "{}";  // данные для config.live.json

#include "file_system.h"
#include "json_config.h"
#include "wifi_init.h"
#include "time_func.h"
#include "ssdp_func.h"
#include "http_func.h"
#include "sensors.h"

void setup()
{
  Serial.begin(115200);
  delay(5);
  Serial.println("");
  //Запускаем файловую систему
  Serial.println("Start 4-FS");
  FS_init();
  configSetup = readFile("config.json", 4096);
  jsonWrite(configJson, "SSDP", jsonRead(configSetup, "SSDP"));
  Serial.println(configSetup);
  Serial.println("Start 1-WIFI");
  //Запускаем WIFI
  WIFIinit();
  Serial.println("Start 8-Time");
  // Получаем время из сети
  Time_init();
  sec_init();
  //Настраиваем и запускаем SSDP интерфейс
  Serial.println("Start 3-SSDP");
  SSDP_init();
  //Настраиваем и запускаем HTTP интерфейс
  Serial.println("Start 2-WebServer");
  HTTP_init();
  DHT_init();
}

void loop()
{
  ts.update();         //планировщик задач
  HTTP.handleClient(); // Работа Web сервера
  yield();
  dnsServer.processNextRequest(); // Для работы DNS в режиме AP
}