#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"

int execute_command(const Command *command);
int execute_pipeline(const Pipeline *pipeline);

#endif
