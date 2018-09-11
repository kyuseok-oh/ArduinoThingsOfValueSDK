#include <Arduino.h>
#include <MQTTClient.h>

typedef struct putContentNode{
	putContentNode *nextNode;
	
}PutContentNode;

boolean tovLoop();
boolean tovConnected();

//For Private Platform
boolean tovConnect(const char clientId[], const char username[], const char password[]);

//For ThingsOfValue Platform
boolean tovConnect();

boolean tovSubscribe();
void tovBegin(Client &net, const char *Oid, const char *DKey, const char *MqttSvr, const char *CseBaseUri, const char *CseUri, MQTTClientCallbackSimple cb);
void putContent(String sensorName, String contentType, String data);
String cmd_get(String cmd, String payload);
void createPutContentJson(const char cseBaseUri[], const char oid[], const char sensorName[], const char contentType[], const char data[]);
