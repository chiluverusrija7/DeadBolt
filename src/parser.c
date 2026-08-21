#include <stdlib.h>
#include <string.h>
#include "parser.h"

static int token_list_push(TokenList *list, const char *token)
{
    if (list->count == list->capacity)
    {
        size_t new_capacity =
            (list->capacity == 0) ? 4 : list->capacity * 2;

        char **new_tokens =
            realloc(list->tokens, new_capacity * sizeof(char *));

        if (new_tokens == NULL)
        {
            return 0;
        }

        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }

    list->tokens[list->count] = malloc(strlen(token) + 1);

    if (list->tokens[list->count] == NULL)
    {
        return 0;
    }

    strcpy(list->tokens[list->count], token);
    list->count++;

    return 1;
}

void token_list_init(TokenList *list)
{
    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
}

int tokenize(const char *input, TokenList *list)
{
    char *copy = malloc(strlen(input) + 1);

    if (copy == NULL)
    {
        return 0;
    }

    strcpy(copy, input);

    char *token = strtok(copy, " ");

    while (token != NULL)
    {
        if (!token_list_push(list, token))
        {
            free(copy);
            return 0;
        }

        token = strtok(NULL, " ");
    }

    free(copy);

    return 1;
}

void token_list_free(TokenList *list)
{
    for (size_t i = 0; i < list->count; i++)
    {
        free(list->tokens[i]);
    }

    free(list->tokens);

    list->tokens = NULL;
    list->count = 0;
    list->capacity = 0;
}

void command_init(Command *command)
{
    token_list_init(&command->arguments);
}

void command_free(Command *command)
{
    token_list_free(&command->arguments);
}

static int pipeline_push_command(Pipeline *pipeline)
{
    if (pipeline->count == pipeline->capacity)
    {
        size_t new_capacity =
            (pipeline->capacity == 0) ? 2 : pipeline->capacity * 2;

        Command *new_commands =
            realloc(pipeline->commands,
                    new_capacity * sizeof(Command));

        if (new_commands == NULL)
        {
            return 0;
        }

        pipeline->commands = new_commands;
        pipeline->capacity = new_capacity;
    }

    command_init(&pipeline->commands[pipeline->count]);
    pipeline->count++;

    return 1;
}

void pipeline_init(Pipeline *pipeline)
{
    pipeline->commands = NULL;
    pipeline->count = 0;
    pipeline->capacity = 0;
}

int parse_pipeline(const char *input, Pipeline *pipeline)
{
    TokenList tokens;

    token_list_init(&tokens);

    if (!tokenize(input, &tokens))
    {
        return 0;
    }

    if (!pipeline_push_command(pipeline))
    {
        token_list_free(&tokens);
        return 0;
    }

    for (size_t i = 0; i < tokens.count; i++)
    {
        if (strcmp(tokens.tokens[i], "|") == 0)
        {
            if (pipeline->commands[pipeline->count - 1].arguments.count == 0)
            {
                token_list_free(&tokens);
                return 0;
            }

            if (!pipeline_push_command(pipeline))
            {
                token_list_free(&tokens);
                return 0;
            }
        }
        else
        {
            Command *current =
                &pipeline->commands[pipeline->count - 1];

            if (!token_list_push(&current->arguments,
                                 tokens.tokens[i]))
            {
                token_list_free(&tokens);
                return 0;
            }
        }
    }

    if (pipeline->commands[pipeline->count - 1].arguments.count == 0)
    {
        token_list_free(&tokens);
        return 0;
    }

    token_list_free(&tokens);

    return 1;
}

void pipeline_free(Pipeline *pipeline)
{
    for (size_t i = 0; i < pipeline->count; i++)
    {
        command_free(&pipeline->commands[i]);
    }

    free(pipeline->commands);

    pipeline->commands = NULL;
    pipeline->count = 0;
    pipeline->capacity = 0;
}
