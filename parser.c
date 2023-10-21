#include<malloc.h>

#include"ast.h"
#include"lexer.h"
#include"util.h"

#define ISIZE_MAIN_PROC 1024

/* return NUM_LITERAL or IDENTIFIER statement and eat the corresponding token
 * */
struct AST* parse_primary_expr(struct Token* tokens, int* i_tokens) {
	struct Token token = tokens[*i_tokens];
	struct AST* expr = get_ast();

	// parse primary expression
	switch (token.type) {
		case T_IDENT: // handle variables
			expr->tag = IDENT;
			expr->data.IDENT.symbol = token.value.vstr;
			(*i_tokens)++;
			break;
		case T_INTEGER: // handle num literals
			expr->tag = NUM_LIT;
			expr->data.NUM_LIT.type = INTEGER;
			expr->data.NUM_LIT.value.vint = token.value.vint;
			(*i_tokens)++;
			break;
		case T_FLOAT: // handle num literals
			expr->tag = NUM_LIT;
			expr->data.NUM_LIT.type = FLOAT;
			expr->data.NUM_LIT.value.vfloat = token.value.vfloat;
			(*i_tokens)++;
			break;
		default:
			raise_err("Error - Unexpected Token: unrecognized token found while parsing: %d", token.type);
	}

	return expr;
}

// FINISH HERE
/*
 * */
struct AST* parse_additive_expr(struct Token* tokens, int* i_tokens) {
	struct AST* left = get_ast();
	left = parse_primary_expr(tokens, i_tokens);

	while (tokens[*i_tokens].type == T_ADD || tokens[*i_tokens].type == T_SUB) {
		struct AST* bin_op = get_ast();
		bin_op->tag = BIN_EXPR; 		// careful: eat token here
		bin_op->data.BIN_EXPR.operand = tokens[(*i_tokens)++].type==T_ADD? '+':'-';
		bin_op->data.BIN_EXPR.vleft = left;
		bin_op->data.BIN_EXPR.vright = parse_primary_expr(tokens, i_tokens);

		left = bin_op;
	}

	return left;
}

struct AST* parse_expr(struct Token* tokens, int* i_tokens) {
	return parse_additive_expr(tokens, i_tokens);
}

struct AST* parse_stmt(struct Token* tokens, int* i_tokens) {
	// nothing to do yet I guess, skip to parse expr

	return parse_expr(tokens, i_tokens);
}

struct AST make_ast(FILE* file) {
    struct Token* tokens = tokenize(file);

    // create the root procedure
    struct AST root;
    root.tag = PROC;
    root.data.PROC.arr = (struct AST**)malloc(ISIZE_MAIN_PROC * sizeof(struct AST*));
    root.data.PROC.max_size = ISIZE_MAIN_PROC;

    int i_proc = 0, i_tokens = 0;

    while (tokens[i_tokens].type != T_EOF) {
    	root.data.PROC.arr[i_proc++] = parse_stmt(tokens, &i_tokens);
    }

    root.data.PROC.size = i_proc;

    // after AST is generated free memory of tokens
    // no need to free generated strings, their pointers are passed to the AST
    free(tokens);

    // remember to free after AST is done:
    // proc arrays
    // string literals
    // keyword/ident strings

    return root;
}

/* order of precedence:
 * primary expr
 * unary expr
 * multiplicative expr
 * additive expr
 * comparative expr
 * logical expr
 * function call
 * member expr
 * assignment expr | done
 * */


