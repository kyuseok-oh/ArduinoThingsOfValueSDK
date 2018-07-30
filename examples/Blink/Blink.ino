// Things of Value(TOV, a.k.a. L.IPS) SDK for Arduino WiFi V1.00 
// Copyright (c) 2018, LDCC. All Rights Reserved.
// Author: Kyuseok Oh

#include <ESP8266WiFi.h>
#include <tov.h>

/////////////////////////////////////////////////////////////////////
// 환경변수 기술
// SDK 사용자에 맞게 수정 필요.
const char ssid[] = "winmin";
const char wifi_pass[] = "11113334";
const char oid[] = "0000000000000000_01033492780";
const char dKey[] = "129f47b7-e92d-3347-f1ad-67681a5145e3";
const char mqttSvr[] = "lottehotel.koreacentral.cloudapp.azure.com";
const char cseBaseUri[] = "/education/base";
const char cseUri[] = "education";
const char mqttUsername[] = "education";
const char mqttPassword[] = "education";
/////////////////////////////////////////////////////////////////////

WiFiClient net;

unsigned long lastMillis = 0;

bool lampStatus = false;
bool wifiStatus = false;
bool mqttStatus = false;

bool runningFuncController = true;

int pinLED = 5;

void setup()
{
  pinMode(pinLED, OUTPUT);

  WiFi.begin(ssid, wifi_pass);
  
  tovBegin(net,oid, dKey, mqttSvr, cseBaseUri, cseUri, messageReceived);

  if(WiFi.status() != WL_CONNECTED){
    wifiStatus = false;
  }else{
    wifiStatus = true;
  }
}

// Blink.
void runningFunc(){
  if(runningFuncController){
    if(lampStatus){
      digitalWrite(pinLED, LOW);
      lampStatus = false;
    }else{
      digitalWrite(pinLED, HIGH);
      lampStatus = true;
    }
  }
}

void loop()
{
  // Running Lotte IoT Platform code in background.
  tovLoop();

  // If platform connection is disconnected, this function reconnect MQTT.
  if(!tovConnected()){
    mqttStatus = false;
    mqttConnect();
  }

  //Running this scope every a second.
  if((millis() - lastMillis) > 1000){
     // Blink Lamp function.
     // Running Blink code in Online.
     runningFunc();

     // Upload status of Lamp.
     if(wifiStatus && mqttStatus){
      if(lampStatus){
        putContent("LAMP", "text/plain", "ON");
      } else{
        putContent("LAMP", "text/plain", "OFF");
      }
    }
    lastMillis = millis();
  }
}

// Get Commands from Platform via MQTT.
void messageReceived(String &topic, String &payload) {
  // "running" is defined command in platform.
  String runningCMD = cmd_get("running", payload);

  if (runningCMD.equals("ON")) {
    runningFuncController = true;
  }
  if (runningCMD.equals("OFF")) {
    runningFuncController = false;
  }
}

// Connect MQTT & WiFi
void mqttConnect() {
  Serial.print("\nchecking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    wifiStatus = false;
    // Running Blink code in Offline.
    runningFunc();
    delay(1000);
  }
  wifiStatus = true;

  //MQTT reconnect
  if (wifiStatus) {
    Serial.print("\nTOV connecting...");
    while(!tovConnect(oid, mqttUsername, mqttPassword)){
      // Running Blink code in Offline.
      runningFunc();
      delay(1000);
    }
    Serial.println("\nConnected!");
    mqttStatus = true;
    
    tovSubscribe();
    Serial.print("\nSuccess subscribe()");
  }
}


