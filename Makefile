CC = gcc
CFLAGS = -Wall -Wextra -std=c11
INCLUDES = -Iinclude

DEADBOLT = build/deadbolt
PROCESS = build/process_generation
VECTOR_TEST = build/vector_test
STRING_TEST = build/string_test

all: $(DEADBOLT) $(PROCESS) $(VECTOR_TEST) $(STRING_TEST)

$(DEADBOLT): src/deadbolt.c
	$(CC) $(CFLAGS) $(INCLUDES) src/deadbolt.c -o $(DEADBOLT)

$(PROCESS): src/process_generation.c
	$(CC) $(CFLAGS) $(INCLUDES) src/process_generation.c -o $(PROCESS)

$(VECTOR_TEST): src/vector.c src/vector_test.c include/vector.h
	$(CC) $(CFLAGS) $(INCLUDES) src/vector.c src/vector_test.c -o $(VECTOR_TEST)

$(STRING_TEST): src/string.c src/string_test.c include/dynamic_string.h
	$(CC) $(CFLAGS) $(INCLUDES) src/string.c src/string_test.c -o $(STRING_TEST)

run: $(DEADBOLT)
	./$(DEADBOLT)

run-process: $(PROCESS)
	./$(PROCESS)

run-vector: $(VECTOR_TEST)
	./$(VECTOR_TEST)

run-string: $(STRING_TEST)
	./$(STRING_TEST)

clean:
	rm -f $(DEADBOLT) $(PROCESS) $(VECTOR_TEST) $(STRING_TEST)
