#include "MQTT_API.h"
#include "project.h"

//#define PRUEBA
#ifdef PRUEBA
#include <sys/time.h>
FILE * fp;
struct timeval cur_time1,cur_time2, tdiff;
#endif
/* MQTT configuration */
#define ADDRESS  "127.0.0.1:1881" //Numero de puerto
char CLIENTID[15];
// Quality of System (0 para no esperar confirmaciones)
int QOS;
#define TIMEOUT     10000L

static MQTTClient client;
static MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
static MQTTClient_message pubmsg = MQTTClient_message_initializer;
static MQTTClient_deliveryToken token;

volatile MQTTClient_deliveryToken deliveredtoken;

/* ****************************************
 * Function name: delivered()
 *
 * Description:
 * 		Confirmation in case QOS>0
 *
 *************************************** */
void delivered(void *context, MQTTClient_deliveryToken dt) {
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
    fflush(stdout);
    fflush(stdin);
}

/* ****************************************
 * Function name: msgarrvd()
 *
 * Description:
 * 		Receives the message and
 * 		show it
 *
 *************************************** */
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
	int i;
	char* payloadptr;
	double a;
	char buff[10096];
	memset(&buff, '0', sizeof(buff));

	printf("Message arrived\n");
	printf("     topic: %s\n", topicName);
	printf("   message: ");
	payloadptr = message->payload;
	for(i=0; i<message->payloadlen; i++)
	{
		a=*payloadptr++;
		putchar(a);
		buff[i]=a;
	}
	buff[message->payloadlen]='\0';
	putchar('\n');
	fflush(stdin);
	fflush(stdout);
    #ifdef PRUEBA
		gettimeofday(&cur_time2,NULL);
		tdiff.tv_sec = cur_time2.tv_sec - cur_time1.tv_sec;
		tdiff.tv_usec = cur_time2.tv_usec - cur_time1.tv_usec;
		printf("Escribiendo...\n");
		//fprintf(fp,"%ld, %ld, %i\n", tdiff.tv_sec,tdiff.tv_usec,message->payloadlen);
		fprintf(fp," %s,",buff);
		fprintf(fp,"%ld\n",cur_time2.tv_sec*1000000+cur_time2.tv_usec);
		fflush(fp);
    #endif
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	return 1;
}

/* ****************************************
 * Function name: connlost()
 *
 * Description:
 * 		For subscriber, it sais if
 * 		the connection with broker is lost
 *
 *************************************** */
void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
    fflush(stdout);
    fflush(stdin);
    #ifdef PRUEBA
    fclose(fp);
    #endif
}



/* ****************************************
 * Function name: MQTT_configuration()
 *
 * Description:
 * 		MQTT client creation and adjust
 * 		connection parameters
 *
 *************************************** */
void MQTT_configuration(int quality){
	if(quality>=3 || quality<0){
		printf("quality error..\n");
	}
	QOS=quality;

	/* Connection parameters */
	conn_opts.keepAliveInterval = 60;
	conn_opts.cleansession = 1;
}

/* ****************************************
 * Function name: MQTT_connect()
 *
 * Description:
 * 		Function connects to MQTT Broker
 * 		with the client name and parameters
 * 		set before
 *
 *************************************** */
void MQTT_connect(char name[]){
	int rc;
	strcpy(CLIENTID, name);
	#ifdef PRUEBA
	fp = fopen("DISTANCIA.txt", "w+");
	#endif
	/* MQTT initialization */
	if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
		MQTTCLIENT_PERSISTENCE_NONE, NULL))!= MQTTCLIENT_SUCCESS) {
		printf("Failed to create client, error %d \n",rc);
		exit(-1);
	}
    	printf("Client created \n");

	/* MQTT callback for subscription */
	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
	        printf("Failed to connect, return code %d\n", rc);
	        exit(-1);
	}
}


/* ****************************************
 * Function name: MQTT_subs()
 *
 * Description:
 * 		Function subscribes to MQTT topic
 * 		to receive the messages post on it
 *
 *************************************** */
void MQTT_subs(char *TOPIC) {
	MQTTClient_subscribe(client, TOPIC, QOS);
	printf("Subscrito a %s\n", TOPIC);
}


/* ****************************************
 * Function name: MQTT_publish()
 *
 * Description:
 * 		Function receives MQTT topic and message
 * 		and send it to the Broker
 *
 *************************************** */
void MQTT_publish(char *PAYLOAD, char *TOPIC){
	int rc;
	/* Publish message */
	pubmsg.payload = PAYLOAD;
	pubmsg.payloadlen = strlen(PAYLOAD);
	pubmsg.qos = QOS;
	pubmsg.retained = 0;
	#ifdef PRUEBA
		gettimeofday(&cur_time1,NULL);
		fprintf(fp,"mensaje %d\n", pubmsg.payloadlen);
		fflush(fp);
	#endif
	MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
	rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
}


/* ****************************************
 * Function name: MQTT_end()
 *
 * Description:
 * 		MQTT client destroy
 *
 *************************************** */
void MQTT_end(){
	/* Disconnect */
	MQTTClient_disconnect(client, 10000);
	MQTTClient_destroy(&client);
	#ifdef PRUEBA
	fclose(fp);
	#endif
}




