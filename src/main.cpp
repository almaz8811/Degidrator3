#include <Arduino.h>
#include <EEPROM.h>
#include "setup.h"
#include "blynk_init.h"
#include "blynk_data.h"

void readData()
{
  EEPROM.get(0, preset);
  EEPROM.get(1, time_sushki);
  EEPROM.get(2, rejim);
  EEPROM.get(3, temp_sushki);
  Blynk.virtualWrite(V0, preset);
  Blynk.virtualWrite(V1, time_sushki);
  Blynk.virtualWrite(V2, rejim);
  Blynk.virtualWrite(V3, temp_sushki);
}


void start()
{
  saveData();
  if (checkStart == 1)
  {
    switch (rejim)
    {
    case 1:
      digitalWrite(14, !HIGH);
      digitalWrite(12, !HIGH);
      digitalWrite(13, !LOW);
      digitalWrite(15, !LOW);
      Blynk.virtualWrite(V5, 255);
      Blynk.virtualWrite(V6, 255);
      Blynk.virtualWrite(V7, 0);
      Blynk.virtualWrite(V8, 0);
      break;
    case 2:
      digitalWrite(14, !HIGH);
      digitalWrite(12, !HIGH);
      digitalWrite(13, !HIGH);
      digitalWrite(15, !LOW);
      Blynk.virtualWrite(V5, 255);
      Blynk.virtualWrite(V6, 255);
      Blynk.virtualWrite(V7, 255);
      Blynk.virtualWrite(V8, 0);
      break;
    case 3:
      digitalWrite(14, !HIGH);
      digitalWrite(12, !HIGH);
      digitalWrite(13, !HIGH);
      digitalWrite(15, !HIGH);
      Blynk.virtualWrite(V5, 255);
      Blynk.virtualWrite(V6, 255);
      Blynk.virtualWrite(V7, 255);
      Blynk.virtualWrite(V8, 255);
      break;
    }
  }
  else
  {
    digitalWrite(14, !LOW);
    digitalWrite(13, !LOW);
    digitalWrite(12, !LOW);
    digitalWrite(15, !LOW);
    Blynk.virtualWrite(V5, 0);
    Blynk.virtualWrite(V6, 0);
    Blynk.virtualWrite(V7, 0);
    Blynk.virtualWrite(V8, 0);
  }
}

void readSens() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V9, h);
  Blynk.virtualWrite(V10, t);
}

void setup()
{
  // put your setup code hegre, to run once:
  Serial.begin(9600);
  Serial.println();
  pinMode(14, OUTPUT); // Верхний вентилятор
  pinMode(12, OUTPUT); // Боковые вентиляторы
  pinMode(13, OUTPUT); // Задний вентилятор
  pinMode(15, OUTPUT); // Подогрев
  blynk_init();
  dht.begin();
  timer.setInterval(1000L, readSens);
  //Blynk.syncAll();
}

void loop()
{
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
  httpServer.handleClient();
  MDNS.update();
  if (millis() - myTimer >= 500)
  {
    start();
    myTimer = millis();
  }
}

void saveData()
{
  EEPROM.put(0, preset);
  EEPROM.put(1, time_sushki);
  EEPROM.put(2, rejim);
  EEPROM.put(3, temp_sushki);
}
