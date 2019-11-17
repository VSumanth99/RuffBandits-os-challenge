CC = gcc
CFLAGS = -lcrypto -std=c99 -lpthread
SRC_FILES = main.c sha_hash.c network.c
EXEC = server
%.o:

all:
	$(CC) $(SRC_FILES) $(CFLAGS) -o $(EXEC)

clean:
	git rm $(EXEC)
