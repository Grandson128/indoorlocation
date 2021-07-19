#include <NTPClient.h>
#include <WiFiUdp.h>

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <set>
#include <string>
#include "./functions.h"

#include <Firebase_ESP_Client.h>
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Define NTP Client to get time
WiFiUDP ntpUDP;
// UTC+1 corresponde a 1*60*60 = 3600
// UTC+2 corresponde a 2*60*60 = 7200
const long utcOffsetInSeconds = 3600;
// NOTA: considerar caso necessário um servidor ntp mais próximo
//NTPClient timeClient(ntpUDP, "0.pt.pool.ntp.org", utcOffsetInSeconds);
NTPClient timeClient(ntpUDP, "0.pt.pool.ntp.org", utcOffsetInSeconds);
// variáveis necessários
#define TIME_MSG_LEN  11   // time sync to PC is HEADER and unix time_t as ten ascii digits
#define TIME_HEADER  255   // Header tag for serial time sync message
#define disable 0
#define enable  1

time_t prevDisplay = 0; // when the digital clock was displayed

unsigned int channel = 1;

/* 1. Define the WiFi credentials */
#ifndef CREDENTIALS
#define mySSID "SSID"
#define myPASSWORD "PASSWORD"
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
struct clientinfo to_send;
boolean send_ = false;


// CallBack para handle dos pacotes wireless recolhidos ou sniffed.
// Esta função está sempre a ser chamada sempre que o modulo Wifi tem promiscuous mode enable
// @param buf   // raw byte buffer packet
// @param len   // tamanho do buffer
void promisc_cb(uint8_t *buf, uint16_t len)
{
  int i = 0;
  uint16_t seq_n_new = 0;
  if (len == 12) {
    struct RxControl *sniffer = (struct RxControl*) buf;
  } else if (len == 128) {
    struct sniffer_buf2 *sniffer = (struct sniffer_buf2*) buf;
    if ((sniffer->buf[0] == 0x40)) {
      struct clientinfo ci = parse_probe(sniffer->buf, 36, sniffer->rx_ctrl.rssi);
        // send to firebase
        // send to firebase
        if (!memcmp(ci.station, rasp_addr, ETH_MAC_LEN)) {
          print_client(ci);
          to_send = ci;
          send_ = true;
        }
    } else if ((sniffer->buf[0] == 0x40)) {
      struct clientinfo ci = parse_probe(sniffer->buf, 36, sniffer->rx_ctrl.rssi);
        // send to firebase
        if (!memcmp(ci.station, rasp_addr, ETH_MAC_LEN)) {
          print_client(ci);
          to_send = ci;
          send_ = true;
        }
    }
  } else {
    struct sniffer_buf *sniffer = (struct sniffer_buf*) buf;
    //Is data or QOS?
    if ((sniffer->buf[0] == 0x08) || (sniffer->buf[0] == 0x88)) {
      struct clientinfo ci = parse_data(sniffer->buf, 36, sniffer->rx_ctrl.rssi, sniffer->rx_ctrl.channel);
      if (memcmp(ci.bssid, ci.station, ETH_MAC_LEN)) {
        // send to firebase
        // send to firebase
        if (!memcmp(ci.station, rasp_addr, ETH_MAC_LEN)) {
          print_client(ci);
          to_send = ci;
          send_ = true;
        }
      }
    }
  }
}

// Esta função é chamada sempre que a variável global _send está igual a true
// Usa o último ClientInfo guardado na referência global to_send para o firebase
void sendToFirebase() {
  taskCompleted = false;
  wifi_promiscuous_enable(disable);
  connectToWiFi();
  Firebase.begin(&config, &auth);
  while (Firebase.ready() && !taskCompleted)
  {
    taskCompleted = true;
    String path = "/beacon/3/signal";
    String jsonStr = "";
    FirebaseJson json1;
    FirebaseJsonData jsonObj;
    // construção do json que servirá de data packet para a RTDB
    json1.set("objectIdentifier", formatMac1(to_send.station));
    json1.set("rssi", to_send.rssi);
    json1.set("datetime", to_send.date_time);
    json1.toString(jsonStr, true);

    // RTDB update
    if (Firebase.RTDB.setJSON(&fbdo, path.c_str(), &json1))
    {
        printResult(fbdo); //see addons/RTDBHelper.h
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

// setup, faz sincronização do timer recorrendo a um servidor NTP
// faz o setup da callback que é chamada em promiscuous mode 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
   
  /* Assign the database URL and database secret(required) */
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;

  wifi_set_opmode(STATION_MODE);            // Promiscuous works only with station mode
  wifi_set_channel(channel);
  wifi_promiscuous_enable(disable);
  synchronize_time();
  wifi_set_promiscuous_rx_cb(promisc_cb);   // Set up promiscuous callback
  delay(100);
  wifi_promiscuous_enable(enable);

 
}

// função para conversão de tempo recebido no NTP para o time do arduino
void synchronize_time() { delay(100);
  connectToWiFi();
  delay(1000);
  timeClient.begin();
  delay(1000);
  timeClient.update();
  int hr, min, sec, day, mnth, yr;
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  mnth = ptm->tm_mon+1;
  yr = ptm->tm_year+1900;
  day = ptm->tm_mday;
  hr = ptm->tm_hour;
  min = ptm->tm_min;
  sec = ptm->tm_sec;
  setTime(hr,min,sec,day,mnth,yr);
  delay(1000);
}

// conexão wireless a um AP
void connectToWiFi() {
  delay(10);
  // We start by connecting to a WiFi network
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(mySSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(mySSID, myPASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
}

// função auxiliar
void printDigits(int digits){
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

// função auxiliar
void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

// percorre os variados channels e chama o sendToFirebase
void loop() {

  channel = 1;
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
  }

  if (send_) {
    sendToFirebase();
    send_ = false;
    delay(100);
    wifi_promiscuous_enable(enable);
  }

}
