#ifndef __COMMAND_H__
#define __COMMAND_H__

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
    void *pridate;
} Command;

/* Get Command structure. */
typedef struct GetCommand {
    char *key;
} GetCommand;

/* Set Command structure. */
typedef struct SetCommand {
    char *key;
    char *value;
} SetCommand;


/* Setx Command structure. */
typedef struct SetxCommand {
    char *key;
    char *value;
    long timeout;
} SetxCommand;

/* Del Command structure. */
typedef struct DelCommand {
    char *key;
} DelCommand;

void pingCommand(int c, void *pridate);
void getCommand(int c, void *pridate);
void setCommand(int c, void *pridate);
void doCommand(int c, char *input);

#endif // !DEBUG
