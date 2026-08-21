#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

typedef struct
{
    char **tokens;
    size_t count;
    size_t capacity;
} TokenList;

typedef struct
{
    TokenList arguments;
} Command;

typedef struct
{
    Command *commands;
    size_t count;
    size_t capacity;
} Pipeline;

void token_list_init(TokenList *list);
int tokenize(const char *input, TokenList *list);
void token_list_free(TokenList *list);

void command_init(Command *command);
void command_free(Command *command);

void pipeline_init(Pipeline *pipeline);
int parse_pipeline(const char *input, Pipeline *pipeline);
void pipeline_free(Pipeline *pipeline);

#endif
