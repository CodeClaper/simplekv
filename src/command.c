#include <stdlib.h>
#include "command.h"
#include "parse.h"
#include "slog.h"

/* Ping. */
void pingCommand(int c, void *pridate) {
    slog(DBUG, "Receive PING.\n");
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
    if (cmd == NULL) return;
    cmd->proc(c, cmd->pridata);
}
