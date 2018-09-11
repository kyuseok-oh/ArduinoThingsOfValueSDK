# ThingsOfValue SDK for Arduino

## 1. What is ThingsOfValue?
***ThingsOfValue*** is an IoT platform of the LOTTE DATA COMMUNICATION COMPANY that complies with the oneM2M global standard, and which can easily develop IoT devices.

## 2. ThingsOfValue SDK for Arduino
***ThingsOfValue SDK for Arduino*** is wrapper of [arduino-mqtt](https://github.com/256dpi/arduino-mqtt) Library for developing device with LOTTE IoT Platform.
This library can be used with many of Arduino-like hardware such as Arduino/Genuino MKR1000 WiFi, NodeMCU(ESP8266) and Arduino/Genuino with WiFi Shields.

## 3. Features
***ThingsOfValue SDK for Arduino*** has the following features:
* Connect to ***ThingsOfValue*** Iot Platform.
* Receive data from the ***ThingsOfValue*** Iot Platform to the device.
* Send data to the ***ThingsOfValue*** Iot Platform.

## 4. Example
The following example is a blink code which controls two LEDs by ***ThingsOfValue*** IoT platform. Each LED is mapped a sensor and an actuator. The sensor is current LED's status. The actuator is used as a switch for each LED.
In the source code below, a nodeMCU (ESP8266) has been used as a device.
```c++
// Things of Value(TOV, a.k.a. L.IPS) SDK for Arduino WiFi V1.01 
// Copyright (c) 2018, LDCC. All Rights Reserved.
// Author: Kyuseok Oh

#include <ESP8266WiFi.h>
#include <tov.h>

/////////////////////////////////////////////////////////////////////
// Environment variables for Using IoT Platform.
const char ssid[] = "Your_WiFi_SSID";
const char wifi_pass[] = "Your_WiFi_Password";
const char oid[] = "Your_Device_OID_in_IoT_Platform";
const char dKey[] = "Your_Device_Key_in_IoT_Platform";
const char mqttSvr[] = "Platform's_MQTT_Server_Address";
const char cseBaseUri[] = "Platform's_CSE_BASE_URI";
const char cseUri[] = "Platform's_CSE_URI";
/////////////////////////////////////////////////////////////////////

WiFiClient net;

unsigned long lastMillis = 0;

bool lampStatusA = false;
bool lampStatusB = true;
bool wifiStatus = false;
bool mqttStatus = false;

bool runningFuncControllerA = true;
bool runningFuncControllerB = true;

int pinLEDA = 5;
int pinLEDB = 4;

void setup()
{
  pinMode(pinLEDA, OUTPUT);
  pinMode(pinLEDB, OUTPUT);
  
  WiFi.begin(ssid, wifi_pass);
  Serial.begin(9600);
  tovBegin(net,oid, dKey, mqttSvr, cseBaseUri, cseUri, messageReceived);

  if(WiFi.status() != WL_CONNECTED){
    wifiStatus = false;
  }else{
    wifiStatus = true;
  }
}

// Blink.
void runningFunc(){
  if(runningFuncControllerA){
    if(lampStatusA){
      digitalWrite(pinLEDA, LOW);
      lampStatusA = false;
    }else{
      digitalWrite(pinLEDA, HIGH);
      lampStatusA = true;
    }
  }
  if(runningFuncControllerB){
    if(lampStatusB){
      digitalWrite(pinLEDB, LOW);
      lampStatusB = false;
    }else{
      digitalWrite(pinLEDB, HIGH);
      lampStatusB = true;
    }
  }
}

void loop()
{
  // Running LOTTE IoT Platform (ThingsOfValue) code in the background.
  tovLoop();

  // If platform connection is disconnected, this function reconnects MQTT.
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
     // "LAMPA" and "LAMPB" are sensors which are defined in the platform.
     if(wifiStatus && mqttStatus){
      if(lampStatusA){
        putContent("LAMPA", "text/plain", "ON");
      } else{
        putContent("LAMPA", "text/plain", "OFF");
      }
      if(lampStatusB){
        putContent("LAMPB", "text/plain", "ON");
      } else{
        putContent("LAMPB", "text/plain", "OFF");
      }
    }
    lastMillis = millis();
  }
}

// Get Commands from Platform via MQTT.
void messageReceived(String &topic, String &payload) {
  // "runningA" and "runningB" are actuators which are defined commands in the platform.
  String runningCMDA = cmd_get("runningA", payload);
  String runningCMDB = cmd_get("runningB", payload);
  if (runningCMDA.equals("ON")) {
    runningFuncControllerA = true;
  }
  if (runningCMDA.equals("OFF")) {
    runningFuncControllerA = false;
  }
  if (runningCMDB.equals("ON")) {
    runningFuncControllerB = true;
  }
  if (runningCMDB.equals("OFF")) {
    runningFuncControllerB = false;
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
    while(!tovConnect()){
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
```

## 5. APIs

Using the following function to initialize device for using the ThingsOfValue IoT platform:
```c++
void tovBegin(Client &net, const char *Oid, const char *DKey, const char *MqttSvr, const char *CseBaseUri, const char *CseUri, MQTTClientCallbackSimple cb);
```
- The 'Oid' is a device ID which was created on the platform.
- The 'DKey' is an authorization key which was automated generated automatically by the platform.



See also the API documentation for the [arduino-mqtt](https://github.com/256dpi/arduino-mqtt) library.
