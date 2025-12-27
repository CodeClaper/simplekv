#include <stdlib.h>
#include "command.h"
#include "eloop.h"
#include "parse.h"
#include "server.h"

/* Ping. */
void pingCommand(int c, void *pridate) {
    if (CreateFileEvent(server.el, c, ELOOP_WRITABLE, ClientSendProc, shared.pong) == ELOOP_ERR) return;
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
