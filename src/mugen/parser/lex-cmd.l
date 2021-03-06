%{
#include <string.h>
#include "parse-cmd.hh"

// #define yydebug printf("yytext is %s at %d\n", yytext, __LINE__);
#define yydebug ;

#define LEX_PREFIX cmd
// #define LEX_VAL(p) LEX_PREFIX##p
#define LEX_VAL(p) cmd##p

// (d|D)(a|A)(t|T)(a|A) { return CMD_DATA; }
// (s|S)(t|T)(a|A)(t|T)(e|E)(d|D)(e|E)(f|F) { return CMD_STATEDEF; }
// (s|S)(t|T)(a|A)(t|T)(e|E) { return CMD_STATE; }

// \[\s*(s|S)(t|T)(a|A)(t|T)(e|E)\s+\d+\s*(,[^\]]*)?\] {

%}

%option yylineno

%%

;[^\n\r]*[\n\r] { yydebug; return COMMENT; }

[\n\r] { yydebug; return LINE_END; }

\[ { yydebug; return LBRACKET; }
\] { yydebug; return RBRACKET; }

\"[^"\n\r]*[^\\]\" { yydebug; LEX_VAL(lval).stringValue = strdup(yytext); return QUOTESTRING; }

\[[ \t]*(s|S)(t|T)(a|A)(t|T)(e|E)[ \t]*-?[0-9]+(,[^\]]*)?\] {
	yydebug;
	LEX_VAL(lval).stringValue = strdup(yytext);
	return CMD_STATE;
}

(s|S)(t|T)(a|A)(t|T)(e|E)(d|D)(e|E)(f|F) { return CMD_STATEDEF; }

([-+])?([0-9]+|[0-9]*\.[0-9]+) { yydebug; LEX_VAL(lval).numberValue = atof(yytext); return NUMBER; }

!= { return CMD_NOTEQ; }
>= { return CMD_GREATERTHANEQ; }
\<= { return CMD_LESSTHANEQ; }
> { return CMD_GREATERTHAN; }
\< { return CMD_LESSTHAN; }

D { return CMD_KEY_DOWN; }
F { return CMD_KEY_FORWARD; }
DF { return CMD_KEY_DOWN_FORWARD; }
B { return CMD_KEY_BACK; }
DB { return CMD_KEY_DOWN_BACK; }
UF { return CMD_KEY_UP_FORWARD; }
U { return CMD_KEY_UP; }
UB { return CMD_KEY_UP_BACK; }

a { return CMD_BUTTON_A; }
b { return CMD_BUTTON_B; }
c { return CMD_BUTTON_C; }
x { return CMD_BUTTON_X; }
y { return CMD_BUTTON_Y; }
z { return CMD_BUTTON_Z; }

&& { return CMD_AND; }
\|\| { return CMD_OR; }

[a-zA-Z_][a-zA-Z0-9_]* { yydebug; LEX_VAL(lval).stringValue = strdup(yytext); return IDENTIFIER; }

[ \t]* { ; }

. { return yytext[0]; }
%%
