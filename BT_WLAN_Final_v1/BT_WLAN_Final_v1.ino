#include <NimBLEDevice.h>
#include <NimBLEScan.h>
#include <NimBLEAdvertisedDevice.h>
#include <WiFi.h>
#include <Wire.h>
#include "esp_wifi.h"

String maclist[256];
int macListSize = 256;
int listcount = 0;
int count = 0;
int cMac80 = 0;
bool isWifiScan = false;

String knownOUIs[] = {
  "00156D", "002722", "0418D6", "18E829", "24A43C", "44D9E7", "687251", "68D79A", "7483C2", "74ACB9", "788A20", "802AA8", "B4FBE4", "DC9FDB", "E063DA", "F09FC2", "F492BF", "FCECDA", "00095B", "000FB5", "00146C", "00184D", "001B2F", "001E2A", "001F33", "00223F", "0024B2", "0026F2", "008EF2", "04A151", "08028E", "0836C9", "08BD43", "100C6B", "100D7F", "10DA43", "1459C0", "200CC8", "204E7F", "20E52A", "288088", "28C68E", "2C3033", "2CB05D", "30469A", "3894ED", "3C3786", "405D82", "4494FC", "44A56E", "4C60DE", "504A6E", "506A03", "6CB0CE", "744401", "78D294", "803773", "841B5E", "8C3BAD", "9C3DCF", "9CC9EB", "9CD36D", "A00460", "A021B7", "A040A0", "A06391", "A42B8C", "B03956", "B07FB9", "B0B98A", "BCA511", "C03F0E", "C0FFD4", "C40415", "C43DC7", "CC40D0", "DCEF09", "E0469A", "E091F5", "E4F4C6", "E8FCAF", "F87394", "000AEB", "001478", "0019E0", "001D0F", "002127", "0023CD", "002586", "002719", "081F71", "085700", "0C4B54", "0C722C", "0C8063", "0C8268", "10FEED", "147590", "148692", "14CC20", "14CF92", "14E6E4", "18A6F7", "18D6C7", "1C3BF3", "1C4419", "1CFA68", "206BE7", "20DCE6", "246968", "282CB2", "28EE52", "30B49E", "30B5C2", "30FC68", "349672", "34E894", "388345", "3C46D8", "40169F", "403F8C", "44B32D", "480EEC", "487D2E", "503EAA", "50BD5F", "50C7BF", "50D4F7", "50FA84", "547595", "54A703", "54C80F", "54E6FC", "5C63BF", "5C899A", "603A7C", "60E327", "645601", "6466B3", "646E97", "647002", "68FF7B", "6CE873", "704F57", "7405A5", "74DA88", "74EA3A", "7844FD", "78A106", "7C8BCA", "7CB59B", "808917", "808F1D", "8416F9", "882593", "8C210A", "8CA6DF", "90AE1B", "90F652", "940C6D", "94D9B3", "984827", "98DAC4", "98DED0", "9C216A", "9CA615", "A0F3C1", "A42BB0", "A8154D", "A8574E", "AC84C6", "B0487A", "B04E26", "B09575", "B0958E", "B0BE76", "B8F883", "BC4699", "BCD177", "C025E9", "C04A00", "C06118", "C0E42D", "C46E1F", "C47154", "C4E984", "CC08FB", "CC32E5", "CC3429", "D03745", "D076E7", "D0C7C0", "D4016D", "D46E0E", "D807B6", "D80D17", "D8150D", "D84732", "D85D4C", "DC0077", "DCFE18", "E005C5", "E4D332", "E894F6", "E8DE27", "EC086B", "EC172F", "EC26CA", "EC888F", "F0F336", "F483CD", "F4EC38", "F4F26D", "F81A67", "F8D111", "FCD733", "000AEB", "001478", "0019E0", "001D0F", "002127", "0023CD", "002586", "002719", "081F71", "085700", "0C4B54", "0C722C", "0C8063", "0C8268", "10FEED", "147590", "148692", "14CC20", "14CF92", "14E6E4", "18A6F7", "18D6C7", "1C3BF3", "1C4419", "1CFA68", "206BE7", "20DCE6", "246968", "282CB2", "28EE52", "30B49E", "30B5C2", "30FC68", "349672", "34E894", "388345", "3C46D8", "40169F", "403F8C", "44B32D", "480EEC", "487D2E", "503EAA", "50BD5F", "50C7BF", "50D4F7", "50FA84", "547595", "54A703", "54C80F", "54E6FC", "5C63BF", "5C899A", "603A7C", "60E327", "645601", "6466B3", "646E97", "647002", "68FF7B", "6CE873", "704F57", "7405A5", "74DA88", "74EA3A", "7844FD", "78A106", "7C8BCA", "7CB59B", "808917", "808F1D", "8416F9", "882593", "8C210A", "8CA6DF", "90AE1B", "90F652", "940C6D", "94D9B3", "984827", "98DAC4", "98DED0", "9C216A", "9CA615", "A0F3C1", "A42BB0", "A8154D", "A8574E", "AC84C6", "B0487A", "B04E26", "B09575", "B0958E", "B0BE76", "B8F883", "BC4699", "BCD177", "C025E9", "C04A00", "C06118", "C0E42D", "C46E1F", "C47154", "C4E984", "CC08FB", "CC32E5", "CC3429", "D03745", "D076E7", "D0C7C0", "D4016D", "D46E0E", "D807B6", "D80D17", "D8150D", "D84732", "D85D4C", "DC0077", "DCFE18", "E005C5", "E4D332", "E894F6", "E8DE27", "EC086B", "EC172F", "EC26CA", "EC888F", "F0F336", "F483CD", "F4EC38", "F4F26D", "F81A67", "F8D111", "FCD733", "00055D", "0080C8", "001349", "0019CB", "0023F8", "00A0C5", "04BF6D", "082697", "107BEF", "1C740D", "28285D", "404A03", "4C9EFF", "4CC53E", "5067F0", "50E039", "54833A", "588BF3", "5C6A80", "5CE28C", "5CF4AB", "603197", "88ACC0", "8C5973", "90EF68", "980D67", "A0E4CB", "B0B2DC", "B8D526", "B8ECA3", "BC9911", "BCCF4F", "C8544B", "C86C87", "CC5D4E", "D8912A", "E4186B", "E8377A", "EC43F6", "FCF528"
};

const wifi_promiscuous_filter_t filt = {
  .filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT | WIFI_PROMIS_FILTER_MASK_DATA
};

typedef struct {
  uint8_t mac[6];
} __attribute__((packed)) MacAddr;

typedef struct {
  int16_t fctl;
  int16_t duration;
  MacAddr da;
  MacAddr sa;
  MacAddr bssid;
  int16_t seqctl;
  unsigned char payload[];
} __attribute__((packed)) WifiMgmtHdr;

#define maxCh 13
int curChannel = 1;

void sniffer(void* buf, wifi_promiscuous_pkt_type_t type) {
  if (!isWifiScan) {
    return;
  }

  wifi_promiscuous_pkt_t *p = (wifi_promiscuous_pkt_t*)buf;
  int len = p->rx_ctrl.sig_len;
  WifiMgmtHdr *wh = (WifiMgmtHdr*)p->payload;
  len -= sizeof(WifiMgmtHdr);
  if (len < 0) {
    Serial.println("Received 0");
    return;
  }

  String packet;
  String mac;
  int fctl = ntohs(wh->fctl);

  for(int i = 8; i <= p->rx_ctrl.sig_len; i++) {
    packet += String(p->payload[i], HEX);
  }

  for(int i = 4; i <= 15; i++) {
    mac += packet[i];
  }
  mac.toUpperCase();

  bool isAP = false;
  String mac_ap = mac.substring(0, 6);
  for (int i = 0; i < sizeof(knownOUIs) / sizeof(knownOUIs[0]); i++) {
    if (mac_ap == knownOUIs[i]) {
      isAP = true;
      break;
    }
  }
  if (!isAP) {
    bool added = false;
    for (int i = 0; i <= listcount; i++) {
      if (mac == maclist[i]) {
        added = true;
      }
    }

    if (!added) {
      maclist[listcount] = mac;
      listcount++;

      int8_t rssi = p->rx_ctrl.rssi;
      Serial.print(mac);
      Serial.print("/");
      Serial.print(rssi);
      Serial.println(";");
    }
  }
}

class MyAdvertisedDeviceCallbacks : public NimBLEAdvertisedDeviceCallbacks {
  void onResult(NimBLEAdvertisedDevice* advertisedDevice) {
    std::string address = advertisedDevice->getAddress().toString();
    address.erase(std::remove(address.begin(), address.end(), ':'), address.end()); 
    String macAddress = String(address.c_str());
    macAddress.toUpperCase(); 
    Serial.print(macAddress);
    Serial.print("/");
    Serial.print(advertisedDevice->getRSSI());
    Serial.println(";");
  }
};

void setup() {
  Serial.begin(115200);
  NimBLEDevice::init("");
  NimBLEScan* pBLEScan = NimBLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(10000);
  pBLEScan->setWindow(99);

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_wifi_set_storage(WIFI_STORAGE_RAM);
  esp_wifi_set_mode(WIFI_MODE_NULL);
  esp_wifi_start();
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_filter(&filt);
  esp_wifi_set_promiscuous_rx_cb(&sniffer);
  esp_wifi_set_channel(curChannel, WIFI_SECOND_CHAN_NONE);
}

void loop() {
  Serial.println("B=====");
  NimBLEScanResults scanResults = NimBLEDevice::getScan()->start(10, false);
  NimBLEDevice::getScan()->clearResults();
  Serial.println("-----");
  Serial.println("W=====");
  isWifiScan = true;
  for(int ci = 0; ci < 13; ci++) {
    if(curChannel > maxCh){
      curChannel = 1;
    }
    esp_wifi_set_channel(curChannel, WIFI_SECOND_CHAN_NONE);
    delay(300);
    char maclist = '\0';
    listcount = 0;
    curChannel++;
  }
  delay(10000);
  isWifiScan = false;
  Serial.println("-----");
}
