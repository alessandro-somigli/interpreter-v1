#include<stdio.h>

#define ISIZE_STR 8
#define ISIZE_IDENT 64
#define ISIZE_TOKEN_ARR 512

enum TokenType {
    // BASE TYPES
    T_INTEGER,
	T_FLOAT,
	T_STR,

    // BINARY OPS
	T_ADD,
	T_SUB,
	T_MUL,
	T_DIV,
	T_MOD,

	T_MORE, // do: from here
	T_LESS,
	T_MORE_EQ,
	T_LESS_EQ,
	T_EQ,
	T_NEQ,

	// UNARY OPS
	T_AND,
	T_OR,
	T_NOT, // end do: to here

    // KEYWORDS
	T_LET,
	// do: add all keywords

    // VARS
	T_ASSIGN,
	T_IDENT,

    // PARENTHESIS
	T_LPAREN,
	T_RPAREN,
	T_LSQUARE,
	T_RSQUARE,
	T_LBRACE,
	T_RBRACE,

    // BREAK
	T_BREAK,

    // EOF
    T_EOF
};

union TokenValue {
    int vint;
    double vfloat;
    char vchar;
    char vkey[8];
    char* vstr;
    void* vptr;
};

struct Token {
    enum TokenType type;
    union TokenValue value;
};

struct Token* tokenize(FILE *file);
