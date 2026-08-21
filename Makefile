CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = build/deadbolt
SOURCE = src/deadbolt.c

all:
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
