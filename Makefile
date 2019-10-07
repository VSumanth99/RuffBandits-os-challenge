CC = gcc
CFLAGS = -lcrypto
SRC_FILES = main.c sha_hash.c network.c

%.o:

all:
	$(CC) $(SRC_FILES) $(CFLAGS) -o server 
