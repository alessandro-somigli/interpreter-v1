#include<malloc.h>

#include"ast.h"
#include"lexer.h"
#include"util.h"

#define ISIZE_MAIN_PROC 1024

struct AST parse_primary_expr(struct Token* tokens, int* i_tokens) {
	struct Token token = tokens[*i_tokens];
	struct AST expr;

	// parse primary expression
	switch (token.type) {
		case T_IDENT: // handle variables
			expr.tag = IDENT;
			expr.data.IDENT.symbol = token.value.vstr;
			(*i_tokens)++;
			break;
		case T_INTEGER: // handle num literals
			expr.tag = NUM_LIT;
			expr.data.NUM_LIT.type = INTEGER;
			expr.data.NUM_LIT.value.vint = token.value.vint;
			(*i_tokens)++;
			break;
		case T_FLOAT: // handle num literals
			expr.tag = NUM_LIT;
			expr.data.NUM_LIT.type = FLOAT;
			expr.data.NUM_LIT.value.vfloat = token.value.vfloat;
			(*i_tokens)++;
			break;
		default:
			raise_err("Error - Unexpected Token: unrecognized token found while parsing: %d", token.type);
	}

	return expr;
}

struct AST parse_expr(struct Token* tokens, int* i_tokens) {
	return parse_primary_expr(tokens, i_tokens);
}

struct AST parse_stmt(struct Token* tokens, int* i_tokens) {
	// nothing to do yet I guess, skip to parse expr

	return parse_expr(tokens, i_tokens);
}

struct AST makeAST(FILE* file) {
    // get tokens
    struct Token* tokens = tokenize(file);

    // create parent procedure
    struct AST root;
    root.tag = PROC;
    root.data.PROC.arr = (struct AST*)malloc(ISIZE_MAIN_PROC * sizeof(struct AST));
    root.data.PROC.size = ISIZE_MAIN_PROC;

    // init indexes
    int i_proc = 0, i_tokens = 0;

    while (tokens[i_tokens].type != T_EOF) {
    	root.data.PROC.arr[i_proc++] = parse_stmt(tokens, &i_tokens);
    }

    // after AST is generated free memory of tokens
    free(tokens);

    // remember to free after AST is done:
    // proc arrays
    // string literals
    // keyword/ident strings

    return root;
}


