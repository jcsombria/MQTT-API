CFLAGS = -Wall -g

client: MQTT_API.o client.o project.h MQTT_API.h
	gcc $(CFLAGS) -o client MQTT_API.o client.o -lm -lwiringPi -lpaho-mqtt3c

MQTT_API.O: MQTT_API.c MQTT_API.h
	gcc $(CFLAGS) MQTT_API.c

proxy: proxy.o
	gcc $(CFLAGS) -o proxy proxy.o -pthread -lm

clean:
	-rm *.o

cleanall:
	rm *.o client proxy
