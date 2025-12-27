#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "command.h"
#include "slog.h"
#include "mm.h"
#include "utils.h"

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern int yylex(void);
extern YY_BUFFER_STATE yy_scan_string(char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern int yyparse(Command *cmd);


/* Parse input and generate command. */
Command *parse(char *input) {
    if (input == NULL) return NULL;

    Trim(input); /* Remove space characters, includes '\f', '\n', '\r', '\t', '\v'*/
    slog(INFO, "Raw input: %s", input);

    size_t size = strlen(input) + 1;
    char buff[size + 1];
    sprintf(buff, "%s%c", input, '\n');
    buff[size] = '\0';

    /* Scan. */
    yy_scan_string(buff);

    Command *cmd = instance(Command);
    return yyparse(cmd) == 0 ? cmd : NULL;
}
