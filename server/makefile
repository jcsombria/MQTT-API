CFLAGS = -Wall -g

proxy: proxy.o
	gcc $(CFLAGS) -o proxy proxy.o -lm -pthread

MQTT_API.o: MQTT_API.c MQTT_API.h
	gcc $(CFLAGS) -c MQTT_API.c -lm -lpaho-mqtt3c

clean:
	-rm *.o proxy
