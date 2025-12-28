#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "dict.h"

typedef void commandProc(int c, void *pridate);

/* All support commands. */
typedef enum CommandType {
    PING,
    GET,
    SET,
    SETX,
    DEL,
    EXISTS,
    INCR,
    DECR,
    KEYS
} CommandType;

/* Command structure. */
typedef struct Command {
    CommandType type;
    commandProc *proc;
    void *pridata;
} Command;

/* Get Command structure. */
typedef struct GetCommand {
    char *key;
} GetCommand;

/* Set Command structure. */
typedef struct SetCommand {
    char *key;
    Node *value;
} SetCommand;


/* Setx Command structure. */
typedef struct SetxCommand {
    char *key;
    Node *value;
    long timeout;
} SetxCommand;

/* Del Command structure. */
typedef struct DelCommand {
    char *key;
} DelCommand;

void pingCommand(int c, void *pridata);
void getCommand(int c, void *pridata);
void setCommand(int c, void *pridata);
void doCommand(int c, char *input);

#endif // !DEBUG
