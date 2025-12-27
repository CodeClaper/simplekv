#include <string.h>
#include "command.h"
#include "parse.h"
#include "server.h"

/* Ping. */
void pingCommand(int c, void *pridate) {
    AddReply(c, shared.pong);
}

/* Get */
void getCommand(int c, void *pridate) {

}

/* Set */
void setCommand(int c, void *pridate) {

}

/* Do command. */
void doCommand(int c, char *input) {
    Command *cmd = parse(input);
    if (cmd == NULL)
        AddReply(c, strdup("ERR, unknown command\r\n"));
    else 
        cmd->proc(c, cmd->pridata);
}
