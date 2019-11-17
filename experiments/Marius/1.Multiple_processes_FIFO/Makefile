CC = gcc
CFLAGS = -lcrypto -std=c99 -lpthread -g
SRC_FILES = main.c sha_hash.c network.c queue.c
EXEC = server
%.o:

all:
	$(CC) $(SRC_FILES) $(CFLAGS) -o $(EXEC)

clean:
	git rm $(EXEC)
