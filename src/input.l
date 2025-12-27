%{
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "parse.h"
#include "utils.h"
#include "slog.h"
#include "y.tab.h"
int lineno = 1;
int yylex();
int yyerror(Command *cmd, const char *s);

char *current_token = NULL;

/* macro to save the text and return a token */
#define TOK(name) { return name; }
%}

%option caseless

%%
PING                    return ping;
GET                     return get;
SET                     return set;
SETX                    return setx;
[-+*/(),.;{}\[\]]	    return yytext[0];
[a-zA-Z][a-zA-Z0-9_]*   { 
                            current_token = strdup(yytext); 
                            yylval.strVal = strdup(yytext); 
                            return IDENTIFIER;
                        } 
[+-]?[0-9]+             { 
                            current_token = strdup(yytext);
                            ST_FLAG flag = StrToLong(yytext, &yylval.intVal);
                            switch (flag) {
                                case ST_SUCCESS:
                                    break;
                                case ST_INVALID:
                                    slog(ERROR, "'%s' is not a valid number", current_token);
                                    break;
                                case ST_OVERFLOW:
                                    slog(ERROR, "'%s' is overflow.", current_token);
                                    break;
                                case ST_OUTRANGE:
                                    slog(ERROR, "'%s' is out of range.", current_token);
                                    break;
                            }
                            return INTVALUE;
                        }
[+-]?[0-9]+	|
[+-]?[0-9]+"."[0-9]* |
"."[0-9]*	            { 
                            current_token = strdup(yytext);
                            ST_FLAG flag = StrToFloat(yytext, &yylval.floatVal);
                            switch (flag) {
                                case ST_SUCCESS:
                                    break;
                                case ST_INVALID:
                                    slog(ERROR, "'%s' is not a valid number", current_token);
                                    break;
                                case ST_OVERFLOW:
                                    slog(ERROR, "'%s' is overflow.", current_token);
                                    break;
                                case ST_OUTRANGE:
                                    slog(ERROR, "'%s' is out of range.", current_token);
                                    break;
                            }
                            return FLOATVALUE; 
                        }
'[^']*$	                { yyerror(NULL, "Unterminated string");  return YYerror;}
\n                      { lineno++; return NL;}
[ \t]                   /* igonore space */
"--".*$		;	        /* comment */
%%

