CC = gcc
CFLAGS = -lcrypto -std=c99 -lpthread -O3 -flto -Iinclude/
SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
EXEC = server
%.o:

all:
	$(CC) $(SRC_FILES) $(CFLAGS) -o $(EXEC)

clean:
	 rm $(EXEC)
	 
