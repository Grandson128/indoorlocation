// This-->tab == "functions.h"

// Expose Espressif SDK functionality
extern "C" {
#include "user_interface.h"
  typedef void (*freedom_outside_cb_t)(uint8 status);
  int  wifi_register_send_pkt_freedom_cb(freedom_outside_cb_t cb);
  void wifi_unregister_send_pkt_freedom_cb(void);
  int  wifi_send_pkt_freedom(uint8 *buf, int len, bool sys_seq);
}

#include <ESP8266WiFi.h>
#include "./structures.h"

uint8_t rasp_addr[ETH_MAC_LEN] = {208, 55, 69, 139, 51, 188};
int nothing_new = 0;

String formatMac1(uint8_t mac[ETH_MAC_LEN]) {
  String hi = "";
  for (int i = 0; i < ETH_MAC_LEN; i++) {
    if (mac[i] < 16) hi = hi + "0" + String(mac[i], HEX);
    else hi = hi + String(mac[i], HEX);
    if (i < 5) hi = hi + ":";
  }
  return hi;
}

String print_client(clientinfo ci)
{
  String hi = "";
  int u = 0;
  int known = 0;   // Clear known flag
  if (ci.err != 0) {
    // nothing
  } else {
    Serial.printf("CLIENT: ");
    Serial.print(formatMac1(ci.station));  //Mac of device
    Serial.printf(" ==> ");

//    for (u = 0; u < aps_known_count; u++)
//    {
//      if (! memcmp(aps_known[u].bssid, ci.bssid, ETH_MAC_LEN)) {
//        //       Serial.print("   ");
//        //        Serial.printf("[%32s]", aps_known[u].ssid);   // Name of connected AP
//        known = 1;     // AP known => Set known flag
//        break;
//      }
//    }
//
//    if (! known)  {
//      Serial.printf("   Unknown/Malformed packet \r\n");
//      for (int i = 0; i < 6; i++) Serial.printf("%02x", ci.bssid[i]);
//    } else {
//      //    Serial.printf("%2s", " ");
      
      Serial.print(formatMac1(ci.ap));   // Mac of connected AP
      Serial.printf("  % 3d", ci.channel);  //used channel
      Serial.printf("   % 4d\r\n", ci.rssi);
//    }
  }
  return hi;
}
