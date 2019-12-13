// WiFiManager

void blynk_init()
{
    //Очистить FS, для теста
    //SPIFFS.format();

    //Считать конфигурацию из FS json
    Serial.println("Монтирование FS...");

    if (SPIFFS.begin())
    {
        Serial.println("mounted file system");
        if (SPIFFS.exists("/config.json"))
        {
            //file exists, reading and loading
            Serial.println("reading config file");
            File configFile = SPIFFS.open("/config.json", "r");
            if (configFile)
            {
                Serial.println("opened config file");
                size_t size = configFile.size();
                // Allocate a buffer to store contents of the file.
                std::unique_ptr<char[]> buf(new char[size]);

                configFile.readBytes(buf.get(), size);
                DynamicJsonBuffer jsonBuffer;
                JsonObject &json = jsonBuffer.parseObject(buf.get());
                json.printTo(Serial);
                if (json.success())
                {
                    Serial.println("\nparsed json");

                    //strcpy(mqtt_server, json["mqtt_server"]);
                    //strcpy(mqtt_port, json["mqtt_port"]);
                    strcpy(blynk_token, json["blynk_token"]);
                }
                else
                {
                    Serial.println("failed to load json config");
                }
            }
        }
    }
    else
    {
        Serial.println("failed to mount FS");
    }
    WiFiManagerParameter custom_blynk_token("blynk", "blynk token", blynk_token, 33);
    WiFiManager wifiManager;
    wifiManager.setSaveConfigCallback(saveConfigCallback);
    wifiManager.addParameter(&custom_blynk_token);
    wifiManager.autoConnect("Degidrator v2.0");
    if (!wifiManager.autoConnect("AutoConnectAP", "password"))
    {
        Serial.println("failed to connect and hit timeout");
        delay(3000);
        //reset and try again, or maybe put it to deep sleep
        ESP.reset();
        delay(5000);
    }
    Serial.println("connected...yeey :)");
    strcpy(blynk_token, custom_blynk_token.getValue());

    if (shouldSaveConfig)
    {
        Serial.println("saving config");
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.createObject();
        //json["mqtt_server"] = mqtt_server;
        //json["mqtt_port"] = mqtt_port;
        json["blynk_token"] = blynk_token;

        File configFile = SPIFFS.open("/config.json", "w");
        if (!configFile)
        {
            Serial.println("failed to open config file for writing");
        }

        json.printTo(Serial);
        json.printTo(configFile);
        configFile.close();
        //end save
    }

    Serial.println("local ip");
    Serial.println(WiFi.localIP());
    // WiFiManager конец
    Blynk.config(blynk_token);
    Blynk.begin(blynk_token, WiFi.SSID().c_str(), WiFi.psk().c_str());

    // Обновление
    MDNS.begin(host);
    httpUpdater.setup(&httpServer);
    httpServer.begin();
    MDNS.addService("http", "tcp", 80);
    Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your браузере\n", host);
}