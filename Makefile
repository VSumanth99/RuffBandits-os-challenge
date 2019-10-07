CC = gcc
CFLAGS = -lcrypto -std=c99
SRC_FILES = main.c sha_hash.c network.c

%.o:

all:
	$(CC) $(SRC_FILES) $(CFLAGS) -o server 
