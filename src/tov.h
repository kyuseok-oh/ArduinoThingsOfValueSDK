#include <Arduino.h>
#include <MQTTClient.h>

boolean tovLoop();
boolean tovConnected();
boolean tovConnect(const char clientId[], const char username[], const char password[]);
boolean tovSubscribe();
void tovBegin(Client &net, const char *Oid, const char *DKey, const char *MqttSvr, const char *CseBaseUri, const char *CseUri, MQTTClientCallbackSimple cb);
void putContent(String sensorName, String contentType, String data);
String cmd_get(const String cmd, const String payload);
String cmd_get_impl(const String cmd, const String payload, int i);
void createPutContentJson(char putContentJson[], const char cseBaseUri[], const char oid[], const char sensorName[], const char contentType[], const char data[]);
