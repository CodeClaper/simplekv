%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parse.h"
#include "command.h"
#include "slog.h"
#include "mm.h"
#include "dict.h"
#include "y.tab.h"

int yywrap() {
    return 1;
}
int yylex(Command *cmd);
int yyerror(Command *cmd, const char *s);
extern char *current_token;
%} 
%union 
{
   char                         *strVal;
   int64_t                      intVal;
   float                        floatVal;
   bool                         boolVal;
   char                         *keyword;
   Node                         *node;
   Command                      *command;
   GetCommand                   *getCommand;
   SetCommand                   *setCommand;
   SetxCommand                  *setxCommand;
   DelCommand                   *delCommand;
};

%left '+' '-'
%left '*' '/'  
%left '(' ')'
%left '[' ']'

%token NL
%token <keyword> ping get set setx del
%token <strVal> IDENTIFIER;
%token <intVal> INTVALUE;
%token <floatVal> FLOATVALUE;
%token <strVal> STRINGVALUE;
%type <strVal> KEY;
%type <node> VALUE;
%type <intVal> TIMEOUT;
%type <command> command;
%type <getCommand> get_command;
%type <setCommand> set_command;
%type <setxCommand> setx_command;
%type <delCommand> del_command;
%parse-param {Command *cmd}
%lex-param {Command *cmd}
%locations
//%define parse.error detailed

%%
command: 
    ping_command
        {
            cmd->type = PING;
            cmd->proc = pingCommand;
            $$ = cmd;
        }
    | get_command 
        {
            cmd->type = GET;
            cmd->proc = getCommand;
            cmd->pridata = $1;
            $$ = cmd;
        }
    | set_command
        {
            cmd->type = SET;
            cmd->proc = setCommand;
            cmd->pridata = $1;
            $$ = cmd;
        }
    | setx_command
        {
            cmd->type = SETX;
            $$ = cmd;
        }
    | del_command
        {
            cmd->type = DEL;
            $$ = cmd;
        }
    ;
ping_command:
    ping END
    ;
get_command:
    get KEY END
        {
            GetCommand *get_cmd = instance(GetCommand);
            get_cmd->key = $2;
            $$ = get_cmd;
        }
    ;
set_command:
    set KEY VALUE END
        {
            SetCommand *set_cmd = instance(SetCommand);
            set_cmd->key = $2;
            set_cmd->value = $3;
            $$ = set_cmd;
        }
    ;
setx_command:
    setx KEY VALUE TIMEOUT END
        {
            SetxCommand *setx_cmd = instance(SetxCommand);
            setx_cmd->key = $2;
            setx_cmd->value = $3;
            setx_cmd->timeout = $4;
            $$ = setx_cmd;
        }
    ;
del_command:
    del KEY END
        {
            DelCommand *del_cmd = instance(DelCommand);
            del_cmd->key = $2;
            $$ = del_cmd;
        }
    ;
KEY:
   IDENTIFIER
        {
            $$ = $1;
        }
   ;
VALUE:
    INTVALUE 
        {
            Node *node = instance(Node);
            node->type = NODE_INT;
            node->value.intVal = $1;
            $$ = node;
        }
    | FLOATVALUE
        {
            Node *node = instance(Node);
            node->type = NODE_FLOAT;
            node->value.floatVal = $1;
            $$ = node;
        }
    | STRINGVALUE
        {
            Node *node = instance(Node);
            node->type = NODE_STRING;
            node->value.strVal = $1;
            $$ = node;
        }
    ;
TIMEOUT:
    INTVALUE
        {
            $$ = $1;
        }
    ;
END:        
    NL
    ;
%%

int yyerror(Command *cmd, const char *s) {
    if (current_token != NULL) {
	    slog(ERROR, "%s at or near [%s].", s, current_token);
        free(current_token);
    }
    else
	    slog(ERROR, "%s.", s);
    return 0;
}
