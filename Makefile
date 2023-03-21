CC = gcc
CFLAGS = -std=c99

all: ipkcpc.c
	$(CC) $(CFLAGS) -o ipkcpc ipkcpc.c


clean: rm ipkcpc

run: all
	./ipkcpc -h localhost -p 1024 -m udp <in

val: all
	valgrind -v ./ipkcpc -h localhost -p 1024 -m udp <in
