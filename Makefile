CC = gcc
CFLAGS = -std=c99

all: $(CC) $(CFLAGS) ipkcpc.c -o ipkcpc


clean: rm ipkcpc
