CC = gcc
CFLAGS = -std=c99

all: ipkcpc.c
	$(CC) $(CFLAGS) -o ipkcpc ipkcpc.c

runudp: all
	./ipkcpc -h localhost -p 2222 -m udp

runtcp: all
	./ipkcpc -h localhost -p 2222 -m tcp

clean: rm ipkcpc
