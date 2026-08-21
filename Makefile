CC = gcc
CFLAGS = -Wall -Wextra -std=c11
INCLUDES = -Iinclude

DEADBOLT = build/deadbolt
PROCESS = build/process_generation
VECTOR_TEST = build/vector_test
STRING_TEST = build/string_test
PARSER_TEST = build/parser_test
EXECUTOR_TEST = build/executor_test

all: $(DEADBOLT) $(PROCESS) $(VECTOR_TEST) $(STRING_TEST) $(PARSER_TEST) $(EXECUTOR_TEST)

$(DEADBOLT): src/deadbolt.c src/parser.c src/executor.c include/parser.h include/executor.h
	$(CC) $(CFLAGS) $(INCLUDES) src/deadbolt.c src/parser.c src/executor.c -o $(DEADBOLT)

$(PROCESS): src/process_generation.c
	$(CC) $(CFLAGS) $(INCLUDES) src/process_generation.c -o $(PROCESS)

$(VECTOR_TEST): src/vector.c src/vector_test.c include/vector.h
	$(CC) $(CFLAGS) $(INCLUDES) src/vector.c src/vector_test.c -o $(VECTOR_TEST)

$(STRING_TEST): src/string.c src/string_test.c include/dynamic_string.h
	$(CC) $(CFLAGS) $(INCLUDES) src/string.c src/string_test.c -o $(STRING_TEST)

$(PARSER_TEST): src/parser.c src/parser_test.c include/parser.h
	$(CC) $(CFLAGS) $(INCLUDES) src/parser.c src/parser_test.c -o $(PARSER_TEST)

$(EXECUTOR_TEST): src/parser.c src/executor.c src/executor_test.c include/parser.h include/executor.h
	$(CC) $(CFLAGS) $(INCLUDES) src/parser.c src/executor.c src/executor_test.c -o $(EXECUTOR_TEST)

run: $(DEADBOLT)
	./$(DEADBOLT)

run-process: $(PROCESS)
	./$(PROCESS)

run-vector: $(VECTOR_TEST)
	./$(VECTOR_TEST)

run-string: $(STRING_TEST)
	./$(STRING_TEST)

run-parser: $(PARSER_TEST)
	./$(PARSER_TEST)

run-executor: $(EXECUTOR_TEST)
	./$(EXECUTOR_TEST)

clean:
	rm -f $(DEADBOLT) $(PROCESS) $(VECTOR_TEST) $(STRING_TEST) $(PARSER_TEST) $(EXECUTOR_TEST)
