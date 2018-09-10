#include <tov.h>

const char *oid;
const char *dkey;
const char *mqttSvr;
const char *cseBaseUri;
const char *cseUri;


char mqttPwd[38];
char subscribeTopic[256];
char publishTopic[256];
char putContentJson[1024];

MQTTClient client(1024);
int qos = 1;

int nodeCount = 0;
PutContentNode *head = NULL;
PutContentNode *tail = NULL;

boolean tovLoop() { return client.loop(); }
boolean tovConnected() { return client.connected(); }
boolean tovConnect(){
	sprintf(mqttPwd, "D|%s", dkey);
	return client.connect(oid, oid, mqttPwd);
}

boolean tovConnect(const char clientId[], const char username[], const char password[]){ return client.connect(clientId, username, password); }

boolean tovSubscribe(){
	boolean unsub = client.unsubscribe(subscribeTopic);
	boolean sub = client.subscribe(subscribeTopic);
	return unsub && sub;
}

void tovBegin(Client &net, const char *Oid, const char *DKey, const char *MqttSvr, const char *CseBaseUri, const char *CseUri, MQTTClientCallbackSimple cb){
	oid = Oid;
	dkey = DKey;
	mqttSvr = MqttSvr;
	cseBaseUri = CseBaseUri;
	cseUri = CseUri;
	sprintf(subscribeTopic, "/oneM2M/req/+/S%s", oid);
 	sprintf(publishTopic, "/oneM2M/req/S%s/%s", oid, cseUri);
	client.begin(mqttSvr, net);
	client.onMessage(cb);
}

void putContent(String sensorName, String contentType, String data) {
  createPutContentJson(cseBaseUri, oid, sensorName.c_str(), contentType.c_str(), data.c_str());
  client.publish(publishTopic, putContentJson, true, qos);
}

void createPutContentJson(const char cseBaseUri[], const char oid[], const char sensorName[], const char contentType[], const char data[]) {
	sprintf(putContentJson, "{\"m2m:rqp\":{\"op\":1,\"to\":\"%s/S%s/%s\",\"fr\":\"/S%s\",\"rqi\":\"rqi-%s-%lu\",\"ty\":4,\"pc\":{\"m2m:cin\":{\"cnf\":\"%s:0\",\"con\":\"%s\"}}}}", cseBaseUri, oid, sensorName, oid, oid, millis(), contentType, data);
}

String cmd_get(String cmd, String payload) {
  int i = 0;
  int j = 0;
  int payload_length = payload.length();
Seeking:
  if ((i + 2) > payload_length) {
    return String("null");
  }
  for (; i < payload_length; i++) {
    if ((payload.charAt(i) == 'n') && (payload.charAt(i + 1) == 'm') && (payload.charAt(i + 2) == '\"')) {
      i = i + 5;  break;
    } else if ((i + 6) > payload_length) {
      return String("null");
    }
  }
  for (; i < payload_length; i++) {
    if ((payload.charAt(i) != '\"') && (payload.charAt(i) != ' ') && (payload.charAt(i) != ',') && (payload.charAt(i) != ':')) {
      break;
    } else if ((i + 5) > payload_length) {
      return String("null");
    }
  }
  if (cmd.equals(payload.substring(i, (i + (cmd.length()))))) {
    for (; i < payload_length; i++) {
      if ((payload.charAt(i) == 'v') && (payload.charAt(i + 1) == 'a') && (payload.charAt(i + 2) == 'l') && (payload.charAt(i + 3) == '\"')) {
        i = i + 4; break;
      } else if ((i + 5) > payload_length) {
        return String("null");
      }
    }
    for (; i < payload_length; i++) {
      if ((payload.charAt(i) != '\"') && (payload.charAt(i) != ' ') && (payload.charAt(i) != ',') && (payload.charAt(i) != ':')) {
        break;
      } else if ((i + 5) > payload_length) {
        return String("null");
      }
    }
    for (j = i + 1; j < payload_length; j++) {
      if (payload.charAt(j) == '\"') {
        break;
      }
    }
    return payload.substring(i, j);
  } else {
    goto Seeking;
  }
}

/*
void enqueNode(String sensorName, String contentType, String data){
	
}
*/
