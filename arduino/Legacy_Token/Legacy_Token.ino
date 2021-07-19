
/**
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt
 * 
 * Copyright (c) 2021 mobizt
 *
*/

/** This example will show how to authenticate using 
 * the legacy token or database secret with the new APIs (using config and auth data).
*/
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

/* 1. Define the WiFi credentials */
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

/* 2. If work with RTDB, define the RTDB URL and database secret */
#define DATABASE_URL "https://indoor-location-se-4383a-default-rtdb.europe-west1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "tzXOapDFPzel9sQdUBEvXAxtgbLUiYf2LGnKY9xb"

/* 3. Define the Firebase Data object */
FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;



bool taskCompleted = false;

void setup()
{

    Serial.begin(9600);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    /* Assign the certificate file (optional) */
    //config.cert.file = "/cert.cer";
    //config.cert.file_storage = StorageType::FLASH;

    /* Assign the database URL and database secret(required) */
    config.database_url = DATABASE_URL;
    config.signer.tokens.legacy_token = DATABASE_SECRET;

    Firebase.reconnectWiFi(true);

    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);
}

void loop()
{

  if (Firebase.ready() && !taskCompleted)
    {
        taskCompleted = true;

        String path = "/beacon/1/signal";

        String jsonStr = "";

        FirebaseJson json1;
        FirebaseJson json2;

        FirebaseJsonData jsonObj;


        json1.set("objectIdentifier", "1");
        json1.set("rssi", "NULL");
        json2.set("", json1);
        
        Serial.println("------------------------------------");
        Serial.println("JSON Data");
        json2.toString(jsonStr, true);
        Serial.println(jsonStr);
        Serial.println("------------------------------------");

        Serial.println("------------------------------------");
        Serial.println("Set JSON test...");


        if (Firebase.RTDB.pushJSON(&fbdo, path.c_str(), &json1))
        {
            Serial.println("PASSED");
            Serial.println("PATH: " + fbdo.dataPath());
            Serial.println("TYPE: " + fbdo.dataType());
            Serial.print("VALUE: ");
            printResult(fbdo); //see addons/RTDBHelper.h
            Serial.println("------------------------------------");
            Serial.println();
        }
        else
        {
            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo.errorReason());
            Serial.println("------------------------------------");
            Serial.println();
        }

        Serial.println("------------------------------------");
        Serial.println("Get JSON test...");

      
        if (Firebase.RTDB.get(&fbdo, path.c_str()))
        {
            Serial.println("PASSED");
            Serial.println("PATH: " + fbdo.dataPath());
            Serial.println("TYPE: " + fbdo.dataType());
            Serial.print("VALUE: ");
            if (fbdo.dataType() == "json")
            {
                printResult(fbdo); //see addons/RTDBHelper.h
            }

            Serial.println("------------------------------------");
            Serial.println();
        }
        else
        {
            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo.errorReason());
            Serial.println("------------------------------------");
            Serial.println();
        }

        
    }
}
