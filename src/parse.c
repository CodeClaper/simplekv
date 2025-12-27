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


/* Parse sql and generate statement list. */
Command *parse(char *sql) {
    if (sql == NULL) return NULL;

    Trim(sql); /* Remove space characters, includes '\f', '\n', '\r', '\t', '\v'*/
    slog(INFO, "Execute sql: %s", sql);

    size_t size = strlen(sql) + 1;
    char buff[size + 1];
    sprintf(buff, "%s%c", sql, '\n');
    buff[size] = '\0';

    /* Scan. */
    yy_scan_string(buff);

    Command *cmd = instance(Command);
    return yyparse(cmd) == 0 ? cmd : NULL;
}
