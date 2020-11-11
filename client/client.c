#include "project.h"
#include <sys/time.h>
struct timeval cur_time;

int main() {
	char topic[] = "top", mess[100000];
	int i;

	memset(mess, 0, sizeof(mess));
        printf("connect\n");

	MQTT_configuration(0);
	MQTT_connect("XBEE1");
	MQTT_subs(topic);
	for(i=0; i<200; i++) {
		delay(10);
		gettimeofday(&cur_time,NULL);
		sprintf(mess, "%ld", cur_time.tv_sec*1000000 + cur_time.tv_usec);
		MQTT_publish(mess, topic); /* Publish message */
	}
	delay(2000);
	MQTT_end();
	return 0;
}
