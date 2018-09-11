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

```

## 5. APIs
