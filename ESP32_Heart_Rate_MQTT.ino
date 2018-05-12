/*  Heart beat monitor for MQTT server
    Heart beat rate is sent to MQTT server every 15sec by default.
    
    I recomment to use ThingsBoard to visualize heart beat rate vs co2 level 
    or temperature or air pressure.

    Author: coniferconifer
    This program is based on the art by pcbreflux ESP32_AD8232.ino at
    https://github.com/pcbreflux/espressif/tree/master/esp32/arduino/sketchbook/ESP32_AD8232
    to get heart beat rate per min.

    Source codes enclosed by #ifdef MQTT is coded by coniferconifer

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, version 3.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#define MQTT  //indicates coded by coniferconifer

#ifdef MQTT
//#define VERBOSE
#define LOFFPLUS GPIO_NUM_16   // Setup for leads off detection LO +
#define LOFFMINUS GPIO_NUM_17 //Setup for leads off detection LO -
#define ANALOGIN GPIO_NUM_34  //ADC for AD8232
#define VERSION "20180512"
#include <WiFi.h>
#include <PubSubClient.h>
#include "credentials.h"
char* ssidArray[] = { WIFI_SSID , WIFI_SSID1, WIFI_SSID2};
char* passwordArray[] = {WIFI_PASS, WIFI_PASS1, WIFI_PASS2};
char* tokenArray[] = { TOKEN , TOKEN1, TOKEN2};
char* serverArray[] = {SERVER, SERVER1, SERVER2};
#define MQTTRETRY 1
#define DEVICE_TYPE "ESP32" // 
String clientId = DEVICE_TYPE ;
char topic[] = "v1/devices/me/telemetry"; //for Thingsboard
#define NTP1 "time.google.com"
#define NTP2 "ntp.nict.jp"
#define NTP3 "ntp.jst.mfeed.ad.jp"
#define MQTTPORT 1883 //for Thingsboard or MQTT server
#define WARMUPTIME 10000 // 10sec
WiFiClient wifiClient;
PubSubClient client(serverArray[0], MQTTPORT, wifiClient);
// index of WiFi access point
int AP = -1;

#define TIMEZONE 9 //in Japan
#define MAX_TRY 15


int initWiFi() {
  int i ;
  int numaccesspt = (sizeof(ssidArray) / sizeof((ssidArray)[0]));

  Serial.print("Number of Access Point = "); Serial.println(numaccesspt);
  for (i = 0;  i < numaccesspt; i++) {
    Serial.print("WiFi connecting to "); Serial.println(ssidArray[i]);
    WiFi.mode(WIFI_OFF);
    WiFi.begin(ssidArray[i], passwordArray[i]);

    int j;
    for (j = 0; j < MAX_TRY; j++) {
      if (WiFi.status() == WL_CONNECTED) {
        int rssi = WiFi.RSSI();
        Serial.printf("RSSI= %d\n", rssi);
        configTime(TIMEZONE * 3600L, 0,  NTP1, NTP2, NTP3);
        Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
        return (i);
      }
      delay(500);
      Serial.print(".");
    }
    Serial.println("can not connect to WiFi AP");
  }
  return (-1);
}



int initWiFi_retry() {
#ifdef VERBOSE
  Serial.print("initWiFi_retry() WiFi connecting to "); Serial.println(ssidArray[AP]);
#endif
  //  Serial.print(" "); Serial.print(passwordArray[AP]);
  WiFi.mode(WIFI_OFF);
  WiFi.begin(ssidArray[AP], passwordArray[AP]);

  int j;
  for (j = 0; j < MAX_TRY; j++) {
    if (WiFi.status() == WL_CONNECTED) {
#ifdef VERBOSE
      Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
#endif
      configTime(TIMEZONE * 3600L, 0,  "time.google.com", "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
      return (AP);
    }
    delay(500);
#ifdef VERBOSE
    Serial.print(".");
#endif
  }
#ifdef VERBOSE
  Serial.println(" can not connect to WiFi AP");
#endif

  return (-1);
}
boolean mqttflag = false;
void publishToMQTT(float beatspermin) {
  // WiFi mode
  String payload = "{";
  payload += "\"beatspermin\":"; payload += beatspermin ;
  payload += "}";

  // dont send too long string to MQTT server
  // max 128byte

#ifdef VERBOSE
  Serial.print("Sending payload: "); Serial.println(payload);
  Serial.print("AP = "); Serial.println(AP);
#endif

#ifdef VERBOSE
  Serial.print("Reconnecting client to "); Serial.println(serverArray[AP]);
#endif
  int mqttloop = 0;

  while (1) { // for thingsboard MQTT server
    mqttflag = client.connect(clientId.c_str(),  tokenArray[AP], NULL);
    if (mqttflag == true) break;
    Serial.print("-"); delay(500);
    mqttloop++;
    if (mqttloop > MQTTRETRY) { //there may be something wrong
      mqttflag = false;
      initWiFi_retry();
      break;
    }
  }

  if (mqttflag == true) {
    if (millis() > WARMUPTIME) {
      if (client.publish(topic, (char*) payload.c_str())) {
#ifdef VERBOSE
        Serial.println("Publish ok");
#endif
      } else {
#ifdef VERBOSE
        Serial.println("Publish failed");
#endif
      }
    } // WARMUP decision end
  } else {
#ifdef VERBOSE
    Serial.println("unable to connect to MQTT server");
#endif
  }

#ifdef WIFI_POWERSAVE
#ifdef VERBOSE
  Serial.println("WiFi turn off");
#endif
  WiFi.mode(WIFI_OFF);
#endif

}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // for Leonardo
  }
  Serial.print("ESP32_Heart_Rate_MQTT: ");
  Serial.println( VERSION );
  pinMode(LOFFPLUS, INPUT); // Setup for leads off detection LO +
  pinMode(LOFFMINUS, INPUT); // Setup for leads off detection LO -

  uint64_t chipid;
  chipid = ESP.getEfuseMac();
  clientId += "-";
  clientId += String((uint32_t)chipid, HEX);
  Serial.println("clientId :" + clientId);

  AP = initWiFi();
  if ( AP != -1) {  // found  WiFi AP
    client.setClient(wifiClient);
    client.setServer(serverArray[AP], MQTTPORT); // MQTT server for NodeRED or MQTT by Thingsboarxd
  }

}
#endif // endof #ifdef MQTT
// heart beat monitor is copied from
// https://github.com/pcbreflux/espressif/tree/master/esp32/arduino/sketchbook/ESP32_AD8232
// Thanks for sharing
/* Copyright (c) 2017 pcbreflux. All Rights Reserved.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, version 3.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef MQTT
#define PLOTT_DATA
// comment out by coniferconifer
#endif
#define MAX_BUFFER 100

uint32_t prevData[MAX_BUFFER];
uint32_t sumData = 0;
uint32_t maxData = 0;
uint32_t avgData = 0;
uint32_t roundrobin = 0;
uint32_t countData = 0;
uint32_t period = 0;
uint32_t lastperiod = 0;
uint32_t millistimer = millis();
float frequency;
float beatspermin = 0;
uint32_t newData;


/*
   This is just a homebrew function.
   Don't take this for critical measurements !!!
   Do your own research on frequencydetection for arbitrary waveforms.
*/
void freqDetec() {
  if (countData == MAX_BUFFER) {
    if (prevData[roundrobin] < avgData * 1.5 && newData >= avgData * 1.5) { // increasing and crossing last midpoint
      period = millis() - millistimer; //get period from current timer value
      millistimer = millis();//reset timer
      maxData = 0;
    }
  }
  roundrobin++;
  if (roundrobin >= MAX_BUFFER) {
    roundrobin = 0;
  }
  if (countData < MAX_BUFFER) {
    countData++;
    sumData += newData;
  } else {
    sumData += newData - prevData[roundrobin];
  }
  avgData = sumData / countData;
  if (newData > maxData) {
    maxData = newData;
  }

  /* Ask your Ask your cardiologist
    how to place the electrodes and read the data!
  */
#ifdef PLOTT_DATA
  Serial.print(newData);
  Serial.print("\t");
  Serial.print(avgData);
  Serial.print("\t");
  Serial.print(avgData * 1.5);
  Serial.print("\t");
  Serial.print(maxData);
  Serial.print("\t");
  Serial.println(beatspermin);
#endif
  prevData[roundrobin] = newData;//store previous value
}
#ifdef MQTT
#define TICK 5 //in msec
#define REPORTTICK 15000 // in msec
long int loopcount = 0;
#endif

void loop() {

  newData = analogRead(34);
  freqDetec();
  if (period != lastperiod) {
    frequency = 1000 / (double)period; //timer rate/period
    if (frequency * 60 > 20 && frequency * 60 < 120) { // supress unrealistic Data
      beatspermin = frequency * 60;
#ifndef MQTT
#ifndef PLOTT_DATA
      Serial.print(frequency);
      Serial.print(" hz");
      Serial.print(" ");
      Serial.print(beatspermin);
      Serial.println(" bpm");
#endif
#endif
      lastperiod = period;
    }
  }
#ifndef MQTT
  delay(5);
#endif
#ifdef MQTT
  delay(TICK);
 
  if ((loopcount % (REPORTTICK / TICK) ) == 0) {
    if ( beatspermin > 50 && beatspermin < 120) { // supress unrealistic Data
      publishToMQTT( beatspermin );
    }
    // print date and time , heart beat rate
    struct tm timeInfo;
    getLocalTime(&timeInfo);
    Serial.printf("Date: %04d/%02d/%02d %02d:%02d:%02d , ",
                  timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
                  timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    Serial.printf("%5.1f \n\r", beatspermin);
  }
   loopcount++;
#endif

}

