#ifndef _MQTTcommunication_H
#define _MQTTcommunication_H

#include "MQTTClient.h"
#include "string.h"
#include <stdlib.h>

void MQTT_configuration();
void MQTT_connect(char *TOPIC);
void MQTT_subs(char *TOPIC);
void MQTT_publish(char *PAYLOAD, char *TOPIC);
void MQTT_end();

void delivered(void *context, MQTTClient_deliveryToken dt);
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);
void connlost(void *context, char *cause);

#endif
