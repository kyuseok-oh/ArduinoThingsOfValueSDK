#include <Arduino.h>
#include <MQTTClient.h>

typedef struct putContentNode{
	putContentNode *nextNode;
	
}PutContentNode;

boolean tovLoop();
boolean tovConnected();

//For Education Platform
boolean tovConnect(const char clientId[], const char username[], const char password[]);
//For TOV
boolean tovConnect();

boolean tovSubscribe();
void tovBegin(Client &net, const char *Oid, const char *DKey, const char *MqttSvr, const char *CseBaseUri, const char *CseUri, MQTTClientCallbackSimple cb);
void putContent(String sensorName, String contentType, String data);
String cmd_get(String cmd, String payload);
String cmd_get_impl(String cmd, String payload, int i);
void createPutContentJson(const char cseBaseUri[], const char oid[], const char sensorName[], const char contentType[], const char data[]);
