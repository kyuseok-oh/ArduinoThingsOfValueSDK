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

Use the following function to initialize device for using the ThingsOfValue IoT platform:
```c++
void tovBegin(Client &net, const char *Oid, const char *DKey, const char *MqttSvr, const char *CseBaseUri, const char *CseUri, MQTTClientCallbackSimple cb);
```
- The `Oid` is a device ID which was created on the platform.
- The `DKey` is an authorization key which was automated generated automatically by the platform.
- The `MqttSvr` is the address of MQTT server.
- `CseBaseUri` and `CseUri` are environment variables for ThingsOfValue platform. They are defined by the platform.
- The `cb` is a callback function that will be run when a message is received from the mqtt server.

To connect to the ***ThingsOfValue*** IoT Platform, use the following function:
```c++
//For ThingsOfValue Platform
boolean tovConnect();

//For Private IoT Platform
boolean tovConnect(const char clientId[], const char username[], const char password[]);
```
- If you want to use the ***ThingsOfValue*** IoT Platform, use the first function without arguments.
- To use private IoT platform or other mqtt brokers, use the second function.
- This function returns a Boolean value indicating whether the connection was successful.

To subscribe a MQTT topic on the ThingsOfValue IoT Platform, use the following function:
```c++
boolean tovSubscribe();
```

Use the following function to check the ***ThingsOfValue*** IoT platform connection of the device:
```c++
boolean tovConnected();
```

Sends and receives packets:
```c++
boolean tovLoop();
```
- This function should be called in every `loop`
- This function is a simple wrapper function of the loop () function of the [arduino-mqtt](https://github.com/256dpi/arduino-mqtt) library.

To send sensor data, use the following function:
```c++
void putContent(String sensorName, String contentType, String data);
```
- The `sensorName` is defined when registering sensors of the device.
- The `contentType` is a MIME type such as `text/plain`.
- Put the value that you want to send to the platform as a String type in the `data` parameter.

To receive a command or a data from ThingsOfValue IoT platform to your device, use the following function:
```c++
String cmd_get(String cmd, String payload);
```
- The `cmd` is defined when registering command keys of actuators of the device on the ***ThingsOfValue*** IoT platform.
- Put the value which is a raw data received from ***ThingsOfValue*** IoT platform via callback function that is defined in `tovBegin` function into the `payload` parameter.

See also the API documentation for the [arduino-mqtt](https://github.com/256dpi/arduino-mqtt) library.
