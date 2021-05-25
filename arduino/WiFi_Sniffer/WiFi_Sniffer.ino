/*
  Copyright 2017 Andreas Spiess

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  This software is based on the work of Ray Burnette: https://www.hackster.io/rayburne/esp8266-mini-sniff-f6b93a
*/

/*
 * @subautores: grupo 8 Sistemas Embutidos FCUP
 * 
 * Houve um aproveitamento do sniffer, e de parte do setup, das funções que fazem handler do buffer e acima de tudo Serial.print
 * Adicionamos o NTPClient e tudo o que é comandos com o Real Time Firebase
 * Durante a fase de recolhimento de documentação, sabiamos que precisamos implentar sniffer com o expressif 8266 non os SDK, ou rtos SDK
 * Este repositório tinha parte das funções que queriamos usar, e por ter algum feedback positivo, e por nos parecer confiável, decidimos reaproveitar porque conhecemos 
 * e reconhecemos as limitações do esp8266(apenas usa um canal de comunição).
 * Temos todo o interesse em aprender mais sobre este modulo, mas com o tempo reduzido que nos restava, foi a solução mais prática e funcional.
 * 
 * 
 * 
 */

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <set>
#include <string>
#include "./functions.h"
#include "./mqtt.h"
#include <Firebase_ESP_Client.h>
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

// Define NTP Client to get time
WiFiUDP ntpUDP;
// UTC+1 corresponde a 1*60*60 = 3600
// UTC+2 corresponde a 2*60*60 = 7200
const long utcOffsetInSeconds = 3600;
// NOTA: considerar caso necessário um servidor ntp mais próximo
NTPClient timeClient(ntpUDP, "0.pt.pool.ntp.org", utcOffsetInSeconds);
// variáveis necessários
#define TIME_MSG_LEN  11   // time sync to PC is HEADER and unix time_t as ten ascii digits
#define TIME_HEADER  255   // Header tag for serial time sync message



#define disable 0
#define enable  1
#define SENDTIME 30000
#define MAXDEVICES 60
#define JBUFFER 15+ (MAXDEVICES * 40)
#define PURGETIME 600000
#define MINRSSI -70

// uint8_t channel = 1;
unsigned int channel = 1;
int clients_known_count_old, aps_known_count_old;
unsigned long sendEntry, deleteEntry;
char jsonString[JBUFFER];
String device[MAXDEVICES];
int nbrDevices = 0;
int usedChannels[15];
StaticJsonBuffer<JBUFFER>  jsonBuffer;


/* 1. Define the WiFi credentials */
#ifndef CREDENTIALS
#define mySSID "MEO-F115E9"
#define myPASSWORD "11509E56F1"
#endif
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



void setup() {
  Serial.begin(9600);
  Serial.printf("\n\nSDK version:%s\n\r", system_get_sdk_version());
  Serial.println(F("Human detector by Andreas Spiess. ESP8266 mini-sniff by Ray Burnette http://www.hackster.io/rayburne/projects"));
  Serial.println(F("Based on the work of Ray Burnette http://www.hackster.io/rayburne/projects"));

  wifi_set_opmode(STATION_MODE);            // Promiscuous works only with station mode
  wifi_set_channel(channel);
  wifi_promiscuous_enable(disable);
  wifi_set_promiscuous_rx_cb(promisc_cb);   // Set up promiscuous callback
  wifi_promiscuous_enable(enable);

  /* Assign the database URL and database secret(required) */
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;
}


void loop() {
  channel = 1;
  boolean sendRTDB = false;
  wifi_set_channel(channel);
  while (true) {
    nothing_new++;                          // Array is not finite, check bounds and adjust if required
    if (nothing_new > 200) {                // monitor channel for 200 ms
      nothing_new = 0;
      channel++;
      if (channel == 15) break;             // Only scan channels 1 to 14
      wifi_set_channel(channel);
    }
    delay(1);  // critical processing timeslice for NONOS SDK! No delay(0) yield()

    if (clients_known_count > clients_known_count_old) {
      clients_known_count_old = clients_known_count;
      sendRTDB = true;
    }
    
    if (aps_known_count > aps_known_count_old) {
      aps_known_count_old = aps_known_count;
      sendRTDB = true;
    }
    
    if (millis() - sendEntry > SENDTIME) {
      sendEntry = millis();
      sendRTDB = true;
    }
  }
  purgeDevice();
  if (sendRTDB) {
    showDevices();
    sendDevicesToFirebase();
  }
}


void connectToWiFi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(mySSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(mySSID, myPASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
}

void purgeDevice() {
  for (int u = 0; u < clients_known_count; u++) {
    if ((millis() - clients_known[u].lastDiscoveredTime) > PURGETIME) {
      Serial.print("purge Client" );
      Serial.println(u);
      for (int i = u; i < clients_known_count; i++) memcpy(&clients_known[i], &clients_known[i + 1], sizeof(clients_known[i]));
      clients_known_count--;
      break;
    }
  }

  for (int u = 0; u < aps_known_count; u++) {
    if ((millis() - aps_known[u].lastDiscoveredTime) > PURGETIME) {
      Serial.print("purge Bacon" );
      Serial.println(u);
      for (int i = u; i < aps_known_count; i++) memcpy(&aps_known[i], &aps_known[i + 1], sizeof(aps_known[i]));
      aps_known_count--;
      break;
    }
  }
}


void showDevices() {
  Serial.println("");
  Serial.println("");
  Serial.println("-------------------Device DB-------------------");
  Serial.printf("%4d Devices + Clients.\n",aps_known_count + clients_known_count); // show count

  // show Beacons
  for (int u = 0; u < aps_known_count; u++) {
    Serial.printf( "%4d ",u); // Show beacon number
    Serial.print("B ");
    Serial.print(formatMac1(aps_known[u].bssid));
    Serial.print(" RSSI ");
    Serial.print(aps_known[u].rssi);
    Serial.print(" channel ");
    Serial.println(aps_known[u].channel);
  }

  // show Clients
  for (int u = 0; u < clients_known_count; u++) {
    Serial.printf("%4d ",u); // Show client number
    Serial.print("C ");
    Serial.print(formatMac1(clients_known[u].station));
    Serial.print(" RSSI ");
    Serial.print(clients_known[u].rssi);
    Serial.print(" channel ");
    Serial.println(clients_known[u].channel);
  }
}



void sendDevicesToFirebase(){
  taskCompleted = false;
  wifi_promiscuous_enable(disable);
  connectToWiFi();
  /* Initialize the library with the Firebase authen and config */
  Firebase.begin(&config, &auth);

  while (Firebase.ready() && !taskCompleted)
  {
    taskCompleted = true;
    String path = "/beacon/1/signal";
    String jsonStr = "";

    FirebaseJson json1;
    FirebaseJson json2;

    FirebaseJsonData jsonObj;

    // manuseamento do datetime ---> possivelemnte colocar no packet snifado
    timeClient.update();
    unsigned long epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime ((time_t *)&epochTime);
    int currentMonth = ptm->tm_mon+1;
    int currentYear = ptm->tm_year+1900;
    int monthDay = ptm->tm_mday;
    int hour = ptm->tm_hour;
    int minutes = ptm->tm_min;
    int sec = ptm->tm_sec;
    String currentDateTime = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay) + " " + String(hour) + "-" + String(minutes) + "-" + String(sec) + ",000";
    Serial.print("Current DateTime: ");
    Serial.println(currentDateTime);
    
    // envia clientes para o RTDB
    for (int u = 0; u < clients_known_count; u++) {

      // construção do json que servirá de data packet para a RTDB
      json1.set("objectIdentifier", formatMac1(clients_known[u].station));
      json1.set("rssi", clients_known[u].rssi);
      json1.set("datetime", currentDateTime);
      delay(1000);
      json2.set("", json1);
      
      Serial.println("------------------------------------");
      Serial.println("JSON Data");
      json1.toString(jsonStr, true);
      Serial.println(jsonStr);
      Serial.println("------------------------------------");

      Serial.println("------------------------------------");
      Serial.println("Set JSON test...");
    
      // RTDB update
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

    // envia Beacons para RTDB
    /*
    for (int u = 0; u < aps_known_count; u++) {
      Serial.printf( "%4d ",u); // Show beacon number
      Serial.print("B ");
      Serial.print(formatMac1(aps_known[u].bssid));
      Serial.print(" RSSI ");
      Serial.print(aps_known[u].rssi);
      Serial.print(" channel ");
      Serial.println(aps_known[u].channel);
    }
    */
  }
  
  delay(100);
  wifi_promiscuous_enable(enable);
  sendEntry = millis();
}
